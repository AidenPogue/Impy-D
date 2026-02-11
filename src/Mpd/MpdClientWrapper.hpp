#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <memory>
#include <optional>
#include <queue>
#include <thread>
#include <mpd/client.h>
#include <mpd/async.h>

#include "ArbitraryTagged.hpp"
#include "ConnectionManager.hpp"
#include "ConnectionManager.hpp"
#include "MpdClientCache.hpp"
#include "MpdIdleEventData.hpp"
#include "MpdSongWrapper.hpp"
#include "../TitleFormatting/ITagged.hpp"
#include "IFilterGenerator.hpp"
#include "ClientActions/IClientAction.hpp"

class MpdClientWrapper
{
public:
    typedef std::function<void(MpdClientWrapper &, MpdIdleEventData &)> MpdClientIdleEventCallback;
    using MpdSongPtr = std::unique_ptr<mpd_song, decltype(&mpd_song_free)>;
    using MpdStatusPtr = std::unique_ptr<mpd_status, decltype(&mpd_status_free)>;

private:
    //Async stuff
    void EventLoop(std::stop_token st);

    ImpyD::Mpd::ConnectionManager connectionManager;

    mutable std::mutex eventsMutex;
    std::condition_variable eventAddedConvar;
    std::queue<std::unique_ptr<ImpyD::ClientActions::IClientAction>> events;
    std::jthread thread;
    void EnqueueEvent(ImpyD::ClientActions::IClientAction *event);

    static std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>> GetQueueImpl(mpd_connection *connection);
public:


    MpdClientWrapper(const char *hostname, unsigned int port);

    [[nodiscard]] bool GetIsConnected() const;

    const ImpyD::Mpd::ConnectionManager &GetConnectionManager() const;

    //Queue
    std::future<std::unique_ptr<MpdSongWrapper>> GetCurrentSong();

    [[nodiscard]] std::future<std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>>> GetQueue();

    void ClearQueue();

    void RandomizeQueue();

    std::future<MpdStatusPtr> GetStatus();

    //Playback

    void PlayCurrent();

    void PlayId(unsigned id);

    void Pause();

    void Toggle();

    void Next();

    void Prev();

    void SeekToSeconds(float s, bool relative);

    void SetVolume(int volume);

    void ChangeVolume(int by);

    //Database
    [[nodiscard]] std::future<std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>>> List(
        std::vector<mpd_tag_type> groups,
        std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator> > filters = {});

    [[nodiscard]] std::future<std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>>> Find(
        std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator> > filters,
        mpd_tag_type sort = MPD_TAG_UNKNOWN);
    void FindAddQueue(std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> filters, mpd_tag_type sort = MPD_TAG_UNKNOWN);
};
