#include "MpdClientWrapper.hpp"

#include <assert.h>
#include <iostream>
#include <poll.h>
#include <mpd/status.h>

#include "Utils.hpp"

static uint8_t *binaryChunkBuffer;
static size_t binaryChunkBufferSize = 0;

MpdClientWrapper::MpdClientWrapper(const char* hostname, uint port, unsigned binaryLimit)
{
    this->hostname = hostname;
    this->port = port;

    Connect();

    /*
    if (GetIsConnected())
    {
        mpd_run_binarylimit(connection, binaryLimit);
        ImMPD::Utils::CreateOrResizeBinaryBuffer(binaryChunkBuffer, binaryChunkBufferSize, binaryLimit);
    }
    */
}

MpdClientWrapper::~MpdClientWrapper()
{
}

void MpdClientWrapper::ThrowIfNotConnected()
{
    if (!GetIsConnected())
    {
        throw 67;
    }
}

void MpdClientWrapper::HandleIdle(mpd_idle idle)
{
    //Reused for all events.
    static MpdIdleEventData eventData;

    eventData.idleEvent = idle;
    eventData.currentSong = GetCurrentSong();
    eventData.currentStatus = GetStatus();

    for (const auto &pair : listeners)
    {
        if (!pair.second)
        {
            continue;
        }

        pair.second(*this, eventData);
    }

    // playInfo.isPlaying = mpd_status_get_state(status) == MPD_STATE_PLAY;
    // playInfo.startedAtElapsedSeconds = mpd_status_get_elapsed_ms(status) / 1000.0f;
    // playInfo.startedAtGlfwTime = glfwGetTime();

    eventData.Free();
}

int MpdClientWrapper::Connect()
{
    if (connection == nullptr)
    {
        connection = mpd_connection_new(hostname, port, 0);
        if (mpd_connection_get_error(connection) != MPD_ERROR_SUCCESS)
        {
            std::cerr << "Connection error: " << mpd_connection_get_error_message(connection) << std::endl;
            mpd_connection_free(connection);
            return mpd_connection_get_error(connection);
        }
        mpd_send_idle(connection);
        return 0;
    }

    return 1;
}


bool MpdClientWrapper::ReceiveIdle()
{
    ThrowIfNotConnected();
    struct pollfd pfd;
    pfd.fd = mpd_connection_get_fd(connection);
    pfd.events = POLLIN;   // data available to read
    pfd.revents = 0;

    int ret = poll(&pfd, 1, 0);

    if (ret > 0 && (pfd.revents & (POLLIN | POLLERR | POLLHUP)))
    {
        mpd_idle idle = mpd_recv_idle(connection, false);
        std::cout << mpd_idle_name(idle) << std::endl;
        HandleIdle(idle);
        return false;
    }
    return !noIdleMode;
}

bool MpdClientWrapper::GetIsConnected()
{
    bool null = connection == nullptr;
    if (!null && mpd_connection_get_error(connection) != MPD_ERROR_SUCCESS)
    {
        std::cout << "Connection error: " << mpd_connection_get_error_message(connection) << std::endl;
        return false;
    }

    return true;
}

int MpdClientWrapper::AddIdleListener(MpdClientIdleEventCallback listener)
{
    auto id = nextListenerId++;
    listeners.emplace(id, listener);
    return id;
}

void MpdClientWrapper::RemoveIdleListener(int id)
{
    if (listeners.find(id) == listeners.end())
    {
        throw std::runtime_error("No listener with id " + std::to_string(id) + " registered.");
    }
    listeners.erase(id);
}

void MpdClientWrapper::BeginNoIdle()
{
    assert(!noIdleMode);
    if (ReceiveIdle())
    {
        mpd_run_noidle(connection);
    }
    noIdleMode = true;
}

void MpdClientWrapper::EndNoIdle()
{
    assert(noIdleMode);
    if (!ReceiveIdle())
    {
        mpd_send_idle(connection);
    }
    noIdleMode = false;
}

mpd_song *MpdClientWrapper::GetCurrentSong()
{
    ThrowIfNotConnected();

    auto song= mpd_run_current_song(connection);
    
    return song;
}

bool MpdClientWrapper::GetQueue(std::vector<mpd_song *> &songList)
{
    auto res = mpd_send_list_queue_meta(connection);
    if (!res)
    {
        return false;
    }
    return ReceiveSongList(songList);
}

bool MpdClientWrapper::PlayCurrent()
{
    ThrowIfNotConnected();

    auto res= mpd_run_play(connection);
    
    return res;
}

bool MpdClientWrapper::PlayId(unsigned id)
{
    ThrowIfNotConnected();
    auto res = mpd_run_play_id(connection, id);
    return res;
}

bool MpdClientWrapper::Pause()
{
    ThrowIfNotConnected();

    auto res= mpd_run_pause(connection, true);
    
    return res;
}

bool MpdClientWrapper::Toggle()
{
    ThrowIfNotConnected();



    auto status = GetStatus();
    auto state = mpd_status_get_state(status);
    mpd_status_free(status);

    bool res = false;

    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN)
    {
        res = mpd_run_play(connection);
    }
    else
    {
        res = mpd_run_pause(connection, state != MPD_STATE_PAUSE);
    }

    
    return res;
}

bool MpdClientWrapper::Next()
{
    ThrowIfNotConnected();

    auto res= mpd_run_next(connection);
    
    return res;
}

bool MpdClientWrapper::Prev()
{
    ThrowIfNotConnected();

    auto res= mpd_run_previous(connection);
    
    return res;
}

bool MpdClientWrapper::SeekToSeconds(float s, bool relative)
{
    ThrowIfNotConnected();

    auto res= mpd_run_seek_current(connection, s, relative);
    
    return res;
}

bool MpdClientWrapper::SetVolume(int volume)
{
    ThrowIfNotConnected();

    auto res= mpd_run_set_volume(connection, volume);
    
    return res;
}

bool MpdClientWrapper::ChangeVolume(int by)
{
    ThrowIfNotConnected();
    return mpd_run_change_volume(connection, by);
}

mpd_status *MpdClientWrapper::GetStatus()
{
    ThrowIfNotConnected();

    auto status = mpd_run_status(connection);
    
    return status;
}

bool MpdClientWrapper::ReceiveSongList(std::vector<mpd_song *> &songList)
{
    auto curSong = mpd_recv_song(connection);
    while (curSong != nullptr)
    {
        songList.push_back(curSong);
        curSong = mpd_recv_song(connection);
    }
    return true;
}

void MpdClientWrapper::Poll()
{
    assert(!noIdleMode);
    if (!ReceiveIdle())
    {
        mpd_send_idle(connection);
    }
}
