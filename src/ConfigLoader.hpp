//
// Created by aiden on 2026-02-12.
//

#ifndef IM_MPD_CONFIGLOADER_HPP
#define IM_MPD_CONFIGLOADER_HPP
#include "Config.hpp"

namespace ImpyD
{
    inline const char *configPath = "impy-d/config.json";

    Config LoadConfigFromDisk();
    void SaveConfig(const Config& config);
}

#endif //IM_MPD_CONFIGLOADER_HPP