#include "ConfigLoader.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

using json = nlohmann::json;

namespace ImpyD
{
    static std::filesystem::path GetUserConfigDir()
    {
        std::filesystem::path path;

        auto xdg = std::getenv("XDG_CONFIG_HOME");
        if (xdg)
        {
            path.append(xdg);
        }
        else
        {
            path.append(std::getenv("HOME"));
        }

        return path;
    }

    static std::filesystem::path GetConfigPath()
    {
        auto dir = GetUserConfigDir();
        dir.append(configPath);
        return dir;
    }

    Config LoadConfigFromDisk()
    {
        std::ifstream f(GetConfigPath());
        json j = json::parse(f);
        Config config = j.get<Config>();
        return config;
    }

    void SaveConfig(const Config& config)
    {
        std::ofstream f(GetConfigPath());
        json j = config;
        f << j.dump(1);
    }
}
