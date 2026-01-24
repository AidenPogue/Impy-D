#include "AlbumArtHelper.hpp"

#include "../Utils.hpp"

namespace ImpyD::Mpd
{
    GLuint AlbumArtHelper::LoadArtworkToGPU(const MpdClientWrapper &client, const std::string &uri, ImVec2 &dimensions, bool tryReadPictureFirst)
    {
        auto data = tryReadPictureFirst ? client.ReadPictureSync(uri) : client.LoadAlbumArtSync(uri);

        if (data.empty())
        {
            data = tryReadPictureFirst ? client.LoadAlbumArtSync(uri) : client.ReadPictureSync(uri);
        }

        if (data.empty())
        {
            //TODO: Load default artwork?
            return 0;
        }

        GLuint tex = 0;
        int width, height;
        Utils::LoadTextureFromMemory(data.data(), data.size(), &tex, &width, &height);
        dimensions.x = width;
        dimensions.y = height;
        return tex;
    }
}
