#include "AlbumArtHelper.hpp"

#include <iostream>

#include "AlbumArtClient.hpp"
#include "../Utils.hpp"
#include "../stb_image.h"

namespace ImpyD::Mpd
{
    static AlbumArtHelper::Result LoadArtworkWorker(AlbumArtClient artClient, std::string uri, bool tryReadPictureFirst)
    {
        auto data = tryReadPictureFirst ? artClient.ReadPicture(uri) : artClient.LoadAlbumArt(uri);

        if (data.empty())
        {
            data = tryReadPictureFirst ? artClient.LoadAlbumArt(uri) : artClient.ReadPicture(uri);
        }

        if (data.empty())
        {
            //TODO: Load default artwork?
            return {};
        }

        int width, height;
        unsigned char *decoded = stbi_load_from_memory((const unsigned char*)data.data(), data.size(), &width, &height, nullptr, 4);

        return AlbumArtHelper::Result(std::unique_ptr<unsigned char>(decoded), width, height);
    }

    std::future<AlbumArtHelper::Result> AlbumArtHelper::LoadArtworkAsync(
        const MpdClientWrapper &client, const std::string &uri,
        bool tryReadPictureFirst)
    {
        return std::async(LoadArtworkWorker, AlbumArtClient(client.GetConnectionManager()), std::string(uri), tryReadPictureFirst);
    }


}
