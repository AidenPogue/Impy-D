#ifndef IM_MPD_ALBUMARTCLIENT_HPP
#define IM_MPD_ALBUMARTCLIENT_HPP
#include <future>
#include <vector>

#include "ConnectionManager.hpp"

namespace ImpyD
{
    /**
     * Client that provides synchronous methods for loading album art from MPD.
     */
    class AlbumArtClient
    {
        Mpd::ConnectionManager connectionManager;

        std::vector<char> LoadAlbumArtSyncImpl(const std::string &uri, bool (*sendFunction) (mpd_connection *, const char *, unsigned));
    public:
        explicit AlbumArtClient(const Mpd::ConnectionManager &existingConnection)
            : connectionManager(existingConnection)
        {
        }

        std::vector<char> LoadAlbumArt(const std::string &uri);

        std::vector<char> ReadPicture(const std::string &uri);
    };
} // ImpyD

#endif //IM_MPD_ALBUMARTCLIENT_HPP