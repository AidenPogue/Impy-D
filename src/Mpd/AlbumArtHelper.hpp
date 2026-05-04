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
        class Result
        {
        public:
            std::unique_ptr<unsigned char> data;
            int width, height;
        };

        /**
         * Tries to load the album art for a song using both albumart and readpicture, falling back on the other if the first fails.
         * @param client
         * @param uri Song URI
         * @param tryReadPictureFirst If true, readpicture will be tried first, followed by albumart if it fails.
         * @return Decoded image data to be loaded to GPU.
         */
        static std::future<Result> LoadArtworkAsync(const MpdClientWrapper &client, const std::string &uri,
                                                    bool tryReadPictureFirst = false);
        
    };
}

#endif //IM_MPD_ALBUMARTHELPER_HPP