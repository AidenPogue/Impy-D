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
    struct MpdClientCache
    {
        MpdSongPtr currentSong = {nullptr, &mpd_song_free};
        MpdStatusPtr status = {nullptr, &mpd_status_free};
    };


    const char *hostname;
    unsigned int port;

    bool noIdleMode = false;

    mpd_connection *connection = nullptr;

    int idleEvents = 0;

    MpdClientCache cache;
    void ClearCache();

    void ThrowIfNotConnected();

    int Connect();

    bool ReceiveIdle();

    std::vector<MpdSongWrapper> ReceiveSongList() const;

    void SetupFind(const std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> *filters, mpd_tag_type sort) const;

public:


    MpdClientWrapper(const char *hostname, unsigned int port, unsigned binaryLimit = 8192);
    ~MpdClientWrapper();

    [[nodiscard]] bool GetIsConnected() const;

    /**
     * Sends the noidle command to MPD. Call this before sending commands *outside an event handler*
     */
    void BeginNoIdle();
    void EndNoIdle();

    /**
     * Gets whether at least one idle event is waiting on this client.
     */
    bool HasIdleEvent();

    /**
     * Gets the bitmask of all the idle events accumulated since the last call to this method and clears it.
     * @return The idle events.
     */
    [[nodiscard]] mpd_idle GetIdleEventsAndClear();


    //Queue
    const MpdSongPtr &GetCurrentSong();

    [[nodiscard]] std::vector<MpdSongWrapper> GetQueue() const;
    bool ClearQueue();
    bool RandomizeQueue();

    const MpdStatusPtr &GetStatus();
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



    void Poll();
};
