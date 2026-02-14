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
            path.append(".config");
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
        auto path = GetConfigPath();
        if (!std::filesystem::exists(path))
        {
            auto defaultConf = Config();
            SaveConfig(defaultConf);
            return defaultConf;
        }

        std::ifstream f(path);
        json j = json::parse(f);
        auto config = j.get<Config>();
        return config;
    }

    void SaveConfig(const Config& config)
    {
        auto path = GetConfigPath();
        if (!std::filesystem::exists(path))
        {
            std::filesystem::create_directories(path);
        }

        std::ofstream f(path);
        json j = config;
        f << j.dump(1);
    }
}
