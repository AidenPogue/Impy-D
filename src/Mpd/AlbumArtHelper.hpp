#ifndef IM_MPD_ALBUMARTHELPER_HPP
#define IM_MPD_ALBUMARTHELPER_HPP
#include <string>

#include "imgui.h"
#include "MpdClientWrapper.hpp"
#include "backends/imgui_impl_opengl3_loader.h"

namespace ImpyD::Mpd
{
    class AlbumArtHelper
    {
    public:
        /**
         * Tries to load the album art for a song using both albumart and readpicture, falling back on the other if the first fails.
         * @param client
         * @param uri Song URI
         * @param dimensions
         * @param tryReadPictureFirst If true, readpicture will be tried first, followed by albumart if it fails.
         * @return Pointer to the loaded texture.
         */
        static GLuint LoadArtworkToGPU(const MpdClientWrapper& client, const std::string &uri, ImVec2 & dimensions, bool tryReadPictureFirst = false);
    };
}

#endif //IM_MPD_ALBUMARTHELPER_HPP