#include "MpdClientWrapper.hpp"
#include "MpdIdleEventData.hpp"

#include <assert.h>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <poll.h>
#include <stack>
#include <mpd/status.h>

#include "ArbitraryTagged.hpp"
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
        mpd_connection_set_keepalive(connection, true);
        if (mpd_connection_get_error(connection) != MPD_ERROR_SUCCESS)
        {
            std::cerr << "Connection error: " << mpd_connection_get_error_message(connection) << std::endl;
            mpd_connection_free(connection);
            connection = nullptr;
            return 1;
        }
        return 0;
    }

    return 1;
}

bool MpdClientWrapper::GetIsConnected() const
{
    bool null = connection == nullptr;
    if (null || mpd_connection_get_error(connection) != MPD_ERROR_SUCCESS)
    {
        //std::cout << "Connection error: " << mpd_connection_get_error_message(connection) << std::endl;
        return false;
    }

    return true;
}

const MpdClientWrapper::MpdSongPtr MpdClientWrapper::GetCurrentSong()
{
    ThrowIfNotConnected();
    return {mpd_run_current_song(connection), &mpd_song_free};
}

std::vector<MpdSongWrapper> MpdClientWrapper::GetQueue() const
{
    mpd_send_list_queue_meta(connection);
    return ReceiveSongList();
}

bool MpdClientWrapper::ClearQueue()
{
    ThrowIfNotConnected();
    return mpd_run_clear(connection);
}

bool MpdClientWrapper::RandomizeQueue()
{
    ThrowIfNotConnected();
    return mpd_run_shuffle(connection);
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

std::vector<ImpyD::Mpd::ArbitraryTagged> MpdClientWrapper::List(
    const std::vector<mpd_tag_type> *groups,
    const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> *filters)
{
    ThrowIfNotConnected();
    mpd_search_cancel(connection);

    assert(!groups->empty());
    mpd_search_db_tags(connection, groups->back());

    if (filters)
    {
        for (auto &expression : *filters)
        {
            expression->ApplyFilter(connection);
        }
    }

    for (int i = groups->size() - 2; i >= 0; i--)
    {
        const auto &group = groups->at(i);
        mpd_search_add_group_tag(connection, group);
    }

    mpd_search_commit(connection);

    std::vector<ImpyD::Mpd::ArbitraryTagged> list;

    //TODO: explain what this is for
    std::vector<std::pair<mpd_tag_type, std::string>> tagStack;

    auto pair = mpd_recv_pair(connection);
    while (pair != nullptr)
    {
        auto currentType = mpd_tag_name_iparse(pair->name);

        if (groups->at(tagStack.size()) == currentType)
        {
            tagStack.emplace_back(std::pair(currentType, pair->value));
        }
        else
        {
            while (tagStack.back().first != currentType)
            {
                tagStack.pop_back();
            }
            tagStack.pop_back();
            tagStack.emplace_back(std::pair(currentType, pair->value));
        }

        if (currentType == groups->back())
        {
            auto item = ImpyD::Mpd::ArbitraryTagged();

            for (const auto &[fst, snd] : tagStack)
            {
                item.AddValue(fst, snd);
            }

            tagStack.pop_back();
            list.push_back(item);
        }

        mpd_return_pair(connection, pair);
        pair = mpd_recv_pair(connection);
    }

    //mpd_response_finish(connection);

    return list;
}

void MpdClientWrapper::SetupFind(const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> *filters, mpd_tag_type sort) const
{

    if (!filters || filters->empty())
    {
        //Fight me, MPD docs. I WILL keep a copy of the entire database
        mpd_search_add_expression(connection, "(any != '')");
    }
    else
    {
        for (auto &expression : *filters)
        {
            expression->ApplyFilter(connection);
        }
    }

    if (sort != MPD_TAG_UNKNOWN)
    {
        mpd_search_add_sort_tag(connection, sort, false);
    }
}

std::vector<MpdSongWrapper> MpdClientWrapper::Find(const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> *filters, mpd_tag_type sort) const
{
    mpd_search_cancel(connection);
    mpd_search_db_songs(connection, true);

    SetupFind(filters, sort);

    mpd_search_commit(connection);

    return ReceiveSongList();
}

void MpdClientWrapper::FindAddQueue(const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> *filters, mpd_tag_type sort) const
{
    mpd_search_cancel(connection);
    mpd_search_add_db_songs(connection, true);

    SetupFind(filters, sort);

    mpd_search_commit(connection);
    mpd_response_finish(connection);
}

std::vector<char> MpdClientWrapper::LoadAlbumArtSync(const std::string &uri) const
{
    return LoadAlbumArtSyncImpl(uri, &mpd_send_albumart);
}

std::vector<char> MpdClientWrapper::ReadPictureSync(const std::string &uri) const
{
    return LoadAlbumArtSyncImpl(uri, &mpd_send_readpicture);
}

std::vector<char> MpdClientWrapper::LoadAlbumArtSyncImpl(const std::string &uri, bool (*sendFunction) (mpd_connection *, const char *, unsigned)) const
{
    sendFunction(connection, uri.c_str(), 0);
    auto sizePair = mpd_recv_pair(connection);

    if (!sizePair)
    {
        mpd_connection_clear_error(connection);
        return {};
    }

    auto buffer = std::vector<char>();
    buffer.resize(std::stoi(sizePair->value));
    mpd_return_pair(connection, sizePair);

    int read = 0;
    int position = 0;

    do
    {
        read = mpd_recv_albumart(connection, buffer.data() + position, buffer.capacity());

        if (read == -1)
        {
            mpd_connection_clear_error(connection);
            return {};
        }

        mpd_response_finish(connection);
        position += read;
        if (read > 0)
        {
            sendFunction(connection, uri.c_str(), position);
        }
    } while (read > 0);

    return buffer;
}

const MpdClientWrapper::MpdStatusPtr MpdClientWrapper::GetStatus()
{
    ThrowIfNotConnected();
    return {mpd_run_status(connection), &mpd_status_free};
}

std::vector<MpdSongWrapper> MpdClientWrapper::ReceiveSongList() const
{
    std::vector<MpdSongWrapper> list;

    auto curSong = mpd_recv_song(connection);
    while (curSong != nullptr)
    {
        list.emplace_back(curSong);
        curSong = mpd_recv_song(connection);
    }

    return list;
}