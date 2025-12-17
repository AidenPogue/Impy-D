#include "MpdClientWrapper.hpp"
#include "MpdIdleEventData.hpp"

#include <assert.h>
#include <iostream>
#include <optional>
#include <poll.h>
#include <mpd/status.h>

#include "MpdSongWrapper.hpp"

static uint8_t *binaryChunkBuffer;
static size_t binaryChunkBufferSize = 0;

MpdClientWrapper::MpdClientWrapper(const char* hostname, unsigned int port, unsigned binaryLimit)
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

void MpdClientWrapper::ClearCache()
{
    cache = MpdClientCache();
}

void MpdClientWrapper::ThrowIfNotConnected()
{
    if (!GetIsConnected())
    {
        throw std::runtime_error(mpd_connection_get_error_message(connection));
    }
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

    if (ret > 0 && pfd.revents & (POLLIN | POLLERR | POLLHUP))
    {
        idleEvents |= mpd_recv_idle(connection, false);
        ClearCache();
        return false;
    }
    return !noIdleMode;
}

bool MpdClientWrapper::GetIsConnected() const
{
    bool null = connection == nullptr;
    if (!null && mpd_connection_get_error(connection) != MPD_ERROR_SUCCESS)
    {
        std::cout << "Connection error: " << mpd_connection_get_error_message(connection) << std::endl;
        return false;
    }

    return true;
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
    ReceiveIdle();
    mpd_send_idle(connection);
    noIdleMode = false;
}

bool MpdClientWrapper::HasIdleEvent()
{
    return idleEvents != 0;
}

mpd_idle MpdClientWrapper::GetIdleEventsAndClear()
{
    auto old = idleEvents;
    idleEvents = 0;
    return static_cast<mpd_idle>(old);
}

const MpdClientWrapper::MpdSongPtr &MpdClientWrapper::GetCurrentSong()
{
    ThrowIfNotConnected();

    if (cache.currentSong == nullptr)
    {
        cache.currentSong = {mpd_run_current_song(connection), &mpd_song_free};
    }
    
    return cache.currentSong;
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

bool MpdClientWrapper::ClearQueue()
{
    ThrowIfNotConnected();
    ClearCache();
    return mpd_run_clear(connection);
}

bool MpdClientWrapper::RandomizeQueue()
{
    ThrowIfNotConnected();
    ClearCache();
    return mpd_run_shuffle(connection);
    mpd_search_c
}

bool MpdClientWrapper::PlayCurrent()
{
    ThrowIfNotConnected();

    auto res= mpd_run_play(connection);
    ClearCache();
    
    return res;
}

bool MpdClientWrapper::PlayId(unsigned id)
{
    ThrowIfNotConnected();
    auto res = mpd_run_play_id(connection, id);
    ClearCache();
    return res;
}

bool MpdClientWrapper::Pause()
{
    ThrowIfNotConnected();
    auto res= mpd_run_pause(connection, true);
    ClearCache();
    return res;
}

bool MpdClientWrapper::Toggle()
{
    ThrowIfNotConnected();
    auto& status = GetStatus();
    auto state = mpd_status_get_state(status.get());

    bool res = false;

    if (state == MPD_STATE_STOP || state == MPD_STATE_UNKNOWN)
    {
        res = mpd_run_play(connection);
    }
    else
    {
        res = mpd_run_pause(connection, state != MPD_STATE_PAUSE);
    }
    ClearCache();
    return res;
}

bool MpdClientWrapper::Next()
{
    ThrowIfNotConnected();
    auto res= mpd_run_next(connection);
    ClearCache();
    return res;
}

bool MpdClientWrapper::Prev()
{
    ThrowIfNotConnected();
    auto res= mpd_run_previous(connection);
    ClearCache();
    return res;
}

bool MpdClientWrapper::SeekToSeconds(float s, bool relative)
{
    ThrowIfNotConnected();
    auto res= mpd_run_seek_current(connection, s, relative);
    ClearCache();
    return res;
}

bool MpdClientWrapper::SetVolume(int volume)
{
    ThrowIfNotConnected();
    auto res= mpd_run_set_volume(connection, volume);
    ClearCache();
    return res;
}

bool MpdClientWrapper::ChangeVolume(int by)
{
    ThrowIfNotConnected();
    ClearCache();
    return mpd_run_change_volume(connection, by);
}

const MpdClientWrapper::MpdStatusPtr &MpdClientWrapper::GetStatus()
{
    ThrowIfNotConnected();

    if (cache.status == nullptr)
    {
        cache.status = {mpd_run_status(connection), &mpd_status_free};
    }
    
    return cache.status;
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