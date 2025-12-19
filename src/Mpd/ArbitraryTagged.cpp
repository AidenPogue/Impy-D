#include "ArbitraryTagged.hpp"

void ImpyD::Mpd::ArbitraryTagged::AddValue(mpd_tag_type key, std::string value)
{
    if (!map.contains(key))
    {
        map.emplace(key, std::vector<std::string>());
    }

    map[key].push_back(value);
}

const std::string &ImpyD::Mpd::ArbitraryTagged::GetSingleValue(mpd_tag_type key) const
{
    if (!map.contains(key))
    {
        return "";
    }

    return map.at(key).front();
}

const std::vector<std::string> &ImpyD::Mpd::ArbitraryTagged::GetAllValues(mpd_tag_type key) const
{
    static std::vector<std::string> emptyValues;

    if (!map.contains(key))
    {
        return emptyValues;
    }

    return map.at(key);
}
