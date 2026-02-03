#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <mpd/client.h>
#include <mpd/async.h>

#include "ArbitraryTagged.hpp"
#include "MpdClientCache.hpp"
#include "MpdIdleEventData.hpp"
#include "MpdSongWrapper.hpp"
#include "../TitleFormatting/ITagged.hpp"
#include "IFilterGenerator.hpp"

class MpdClientWrapper
{
public:
    typedef std::function<void(MpdClientWrapper &, MpdIdleEventData &)> MpdClientIdleEventCallback;
    using MpdSongPtr = std::unique_ptr<mpd_song, decltype(&mpd_song_free)>;
    using MpdStatusPtr = std::unique_ptr<mpd_status, decltype(&mpd_status_free)>;

private:
    const char *hostname;
    unsigned int port;

    mpd_connection *connection = nullptr;

    void ThrowIfNotConnected();

    int Connect();

    std::vector<MpdSongWrapper> ReceiveSongList() const;

    void SetupFind(const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> *filters, mpd_tag_type sort) const;

    std::vector<char> LoadAlbumArtSyncImpl(const std::string &uri, bool (*sendFunction)(mpd_connection*, const char*, unsigned)) const;

public:


    MpdClientWrapper(const char *hostname, unsigned int port, unsigned binaryLimit = 8192);
    ~MpdClientWrapper();

    [[nodiscard]] bool GetIsConnected() const;

    //Queue
    const MpdSongPtr GetCurrentSong();

    [[nodiscard]] std::vector<MpdSongWrapper> GetQueue() const;
    bool ClearQueue();
    bool RandomizeQueue();

    const MpdStatusPtr GetStatus();
    //bool StartAlbumArt(const char *uri, )

    //Playback

    bool PlayCurrent();
    bool PlayId(unsigned id);
    bool Pause();
    bool Toggle();
    bool Next();
    bool Prev();
    bool SeekToSeconds(float s, bool relative);

    bool SetVolume(int volume);
    bool ChangeVolume(int by);

    //Database
    [[nodiscard]] std::vector<ImpyD::Mpd::ArbitraryTagged> List(
        const std::vector<mpd_tag_type> *groups,
        const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator> > *filters = nullptr);

    [[nodiscard]] std::vector<MpdSongWrapper> Find(const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> *filters = nullptr, mpd_tag_type sort = MPD_TAG_UNKNOWN) const;
    void FindAddQueue(const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> *filters = nullptr, mpd_tag_type sort = MPD_TAG_UNKNOWN) const;



    //Artwork
    std::vector<char> LoadAlbumArtSync(const std::string &uri) const;
    std::vector<char> ReadPictureSync(const std::string &uri) const;
};
