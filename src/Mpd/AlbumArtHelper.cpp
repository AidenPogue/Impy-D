#include "AlbumArtHelper.hpp"

#include "../Utils.hpp"

namespace ImpyD::Mpd
{
    GLuint AlbumArtHelper::LoadArtworkToGPU(const MpdClientWrapper &client, const std::string &uri, ImVec2 &dimensions, bool tryReadPictureFirst)
    {
        auto data = tryReadPictureFirst ? client.ReadPictureSync(uri) : client.LoadAlbumArtSync(uri);

        if (data.capacity() == 0)
        {
            data = tryReadPictureFirst ? client.LoadAlbumArtSync(uri) : client.ReadPictureSync(uri);
        }

        if (data.capacity() == 0)
        {
            //TODO: Load default artwork?
            return 0;
        }

        GLuint tex = 0;
        int width, height;
        Utils::LoadTextureFromMemory(data.data(), data.capacity(), &tex, &width, &height);
        dimensions.x = width;
        dimensions.y = height;
        return tex;
    }
}
