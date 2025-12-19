#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <optional>
#include <mpd/client.h>
#include <mpd/async.h>

#include "MpdClientCache.hpp"
#include "MpdIdleEventData.hpp"
#include "../TitleFormatting/ITagged.hpp"

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
    bool ReceiveSongList(std::vector<mpd_song *> &songList);

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

    const MpdSongPtr &GetCurrentSong();
    bool GetQueue(std::vector<mpd_song *> &songList);
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
    std::vector<std::unique_ptr<ImpyD::TitleFormatting::ITagged>> List(mpd_tag_type mainTag,
                                                                       const std::unique_ptr<std::vector<std::string> >
                                                                       &filters = nullptr,
                                                                       const std::unique_ptr<std::vector<mpd_tag_type> >
                                                                       &groups = nullptr);

    void Poll();
};
