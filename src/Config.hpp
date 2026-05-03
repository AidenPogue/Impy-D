#ifndef IM_MPD_CONFIG_HPP
#define IM_MPD_CONFIG_HPP
#include "IConfigItem.hpp"
#include <string>
#include <nlohmann/json.hpp>

#include "panels/LibraryLayer.hpp"

namespace ImpyD
{
    class Config
    {

    public:
        class Interface : public IConfigItem
        {
        public:
            /**
             * Format to use with the current song for the window title.
             */
            std::string windowTitleFormat = "%title% - %artist% | Impy-D";

            bool DrawEditor() override;

            std::string GetDisplayName() override;
        };

        class Library : public IConfigItem
        {
        public:
            bool DrawEditor() override;

            std::string GetDisplayName() override;

            class LibraryView : public IConfigItem
            {
            public:
                LibraryView() = default;
                LibraryView(std::string name, std::vector<LibraryLayer> layers)
                    : name(std::move(name)),
                      layers(std::move(layers))
                {
                }

                bool DrawEditor() override;

                std::string GetDisplayName() override;

                std::string name;
                std::vector<LibraryLayer> layers;
            };

            std::vector<LibraryView> libraryViews =
            {
                LibraryView("AlbumArtist > Album > Disc > Song", {
                                LibraryLayer("%albumartist%", MPD_TAG_ALBUM_ARTIST),
                                LibraryLayer("%album% (%date%)", MPD_TAG_DATE),
                                LibraryLayer("Disc %disc%", MPD_TAG_DISC, true),
                                LibraryLayer("%disc%.%track% - %title% - %artist% (%duration%)", MPD_TAG_TRACK)
                            })
            };
        };

        class Visualizer : public IConfigItem
        {
        public:
            std::string GetDisplayName() override;

            bool DrawEditor() override;

            std::string fifoPath = "/tmp/mpd.fifo";
            int fifoSampleRate = 44100;
            int fifoDepth = 16;
            int fifoChannels = 2;

            int bufferSize = 44100 / 2;
        };

        Interface interface;
        Library library;
        Visualizer visualizer;

    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config::Interface, windowTitleFormat);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config, interface, library, visualizer)

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config::Library, libraryViews);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config::Library::LibraryView, name, layers);

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config::Visualizer, fifoPath, fifoSampleRate, fifoDepth, fifoChannels);
}

#endif