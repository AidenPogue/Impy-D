#include "AlbumArtClient.hpp"

#include <iostream>
#include <mpd/client.h>

namespace ImpyD
{
    std::vector<char> AlbumArtClient::LoadAlbumArt(const std::string &uri)
    {
        return LoadAlbumArtSyncImpl(uri, &mpd_send_albumart);
    }

    std::vector<char> AlbumArtClient::ReadPicture(const std::string &uri)
    {
        return LoadAlbumArtSyncImpl(uri, &mpd_send_readpicture);
    }

    std::vector<char> AlbumArtClient::LoadAlbumArtSyncImpl(const std::string &uri, bool (*sendFunction) (mpd_connection *, const char *, unsigned))
    {
        auto connection = connectionManager.GetConnection();
        std::cout << uri << std::endl;
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
}
