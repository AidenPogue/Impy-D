#ifndef IM_MPD_CONFIG_HPP
#define IM_MPD_CONFIG_HPP
#include "IConfigItem.hpp"
#include <string>
#include <nlohmann/json.hpp>

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


        };

        Interface interface;

    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config::Interface, windowTitleFormat);
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(Config, interface)
}

#endif