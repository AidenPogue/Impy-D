#include "MpdClientWrapper.hpp"

#include <algorithm>

#include "MpdIdleEventData.hpp"

#include <fstream>
#include <future>
#include <iostream>
#include <utility>
#include <mpd/status.h>

#include "ArbitraryTagged.hpp"
#include "imgui.h"
#include "MpdSongWrapper.hpp"
#include "../Utils.hpp"
#include "ClientActions/ChangeVolumeAction.hpp"
#include "ClientActions/FindAction.hpp"
#include "ClientActions/GetCurrentSongAction.hpp"
#include "ClientActions/ListAction.hpp"
#include "ClientActions/NoParameterAction.hpp"
#include "ClientActions/NoParameterPromiseAction.hpp"
#include "ClientActions/PlayIdAction.hpp"
#include "ClientActions/SeekAction.hpp"

using namespace ImpyD::ClientActions;
using namespace std::chrono_literals;

#define NO_PARAM_ACTION_PTR(func) new NoParameterAction(func)

MpdClientWrapper::MpdClientWrapper(const char* hostname, unsigned int port) :
connectionManager(hostname, port),
thread([this](std::stop_token st) { EventLoop(st); })
{
}

void MpdClientWrapper::EventLoop(std::stop_token st)
{
    //This was not chosen in any kind of scientific way.
    //It could probably be a lot longer, but I'm not sure by how much.
    const auto pingInterval = 5s;

    std::cout << "Starting main client loop." << std::endl;
    while (!st.stop_requested())
    {
        std::unique_lock lk(eventsMutex);
        eventAddedConvar.wait_for(lk, pingInterval, [this](){return !events.empty();});

        if (connectionManager.CheckConnected())
        {
            auto connection = connectionManager.GetConnection();

            //If we woke up and are still empty it means we timed out.
            //We periodically ping to stop the connection from getting closed.
            if (events.empty())
            {
                mpd_send_command(connection, "ping", nullptr);
                mpd_response_finish(connection);
            }
            else
            {
                const auto &ev = events.front();
                ev->Execute(connection);
                events.pop();
            }
        }

        lk.unlock();
    }
}

bool MpdClientWrapper::GetIsConnected() const
{
    return connectionManager.CheckConnected();
}

const ImpyD::Mpd::ConnectionManager & MpdClientWrapper::GetConnectionManager() const
{
    return connectionManager;
}

std::future<std::unique_ptr<MpdSongWrapper>> MpdClientWrapper::GetCurrentSong()
{
    auto action = new NoParameterPromiseAction<std::unique_ptr<MpdSongWrapper>>(
        [](mpd_connection *c)
        {
            auto s = mpd_run_current_song(c);
            return s ? std::make_unique<MpdSongWrapper>(s) : nullptr;
        });
    auto future = action->GetFuture();
    EnqueueEvent(action);
    return future;
}

std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>> MpdClientWrapper::GetQueueImpl(mpd_connection *connection)
{
    mpd_send_list_queue_meta(connection);
    return ImpyD::Utils::ReceiveSongList(connection);
}

std::future<std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>>> MpdClientWrapper::GetQueue()
{
    auto action = new NoParameterPromiseAction(GetQueueImpl);
    auto future = action->GetFuture();
    EnqueueEvent(action);
    return future;
}

void MpdClientWrapper::ClearQueue()
{
    EnqueueEvent(NO_PARAM_ACTION_PTR(mpd_run_clear));
}

void MpdClientWrapper::RandomizeQueue()
{
    EnqueueEvent(NO_PARAM_ACTION_PTR(mpd_run_shuffle));
}

void MpdClientWrapper::PlayCurrent()
{
    EnqueueEvent(NO_PARAM_ACTION_PTR(mpd_run_play));
}

void MpdClientWrapper::PlayId(unsigned id)
{
    EnqueueEvent(new PlayIdAction(id));
}

void MpdClientWrapper::Pause()
{
    EnqueueEvent(NO_PARAM_ACTION_PTR([](mpd_connection *c){return mpd_run_pause(c, true);}));
}

static bool ToggleImpl(mpd_connection *connection)
{
    auto status = mpd_run_status(connection);
    auto state = mpd_status_get_state(status);
    mpd_status_free(status);

    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN)
    {
        return mpd_run_play(connection);
    }

    return mpd_run_pause(connection, state != MPD_STATE_PAUSE);
}

void MpdClientWrapper::Toggle()
{
    EnqueueEvent(NO_PARAM_ACTION_PTR(ToggleImpl));
}

void MpdClientWrapper::Next()
{
    EnqueueEvent(NO_PARAM_ACTION_PTR(mpd_run_next));
}

void MpdClientWrapper::Prev()
{
    EnqueueEvent(NO_PARAM_ACTION_PTR(mpd_run_previous));
}

void MpdClientWrapper::SeekToSeconds(float s, bool relative)
{
    EnqueueEvent(new SeekAction(s, relative));
}

void MpdClientWrapper::SetVolume(int volume)
{
    EnqueueEvent(new ChangeVolumeAction(volume, ChangeVolumeAction::Mode::Set));
}

void MpdClientWrapper::ChangeVolume(int by)
{
    EnqueueEvent(new ChangeVolumeAction(by, ChangeVolumeAction::Mode::ChangeBy));
}


std::future<std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>>> MpdClientWrapper::List(
    std::vector<mpd_tag_type> groups,
    std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator> > filters)
{
    auto event = new ListAction(std::move(groups), std::move(filters));
    auto future = event->GetFuture();
    EnqueueEvent(event);
    return future;
}

std::future<std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>>> MpdClientWrapper::Find(
    std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> filters, mpd_tag_type sort)
{
    auto event = new FindAction(std::move(filters), sort, false);
    auto future = event->GetFuture();
    EnqueueEvent(event);
    return future;
}

void MpdClientWrapper::FindAddQueue(std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> filters, mpd_tag_type sort)
{
    EnqueueEvent(new FindAction(std::move(filters), sort, true));
}

std::future<MpdClientWrapper::MpdStatusPtr> MpdClientWrapper::GetStatus()
{
    auto action = new NoParameterPromiseAction<MpdStatusPtr>([](mpd_connection *c) -> MpdStatusPtr {return {mpd_run_status(c), mpd_status_free};});
    auto future = action->GetFuture();
    EnqueueEvent(action);
    return future;
}

void MpdClientWrapper::EnqueueEvent(IClientAction *event)
{
    eventsMutex.lock();

    events.emplace(event);

    eventsMutex.unlock();
    eventAddedConvar.notify_one();
}