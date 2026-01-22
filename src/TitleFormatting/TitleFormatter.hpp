#ifndef IM_MPD_TITLEFORMATTER_HPP
#define IM_MPD_TITLEFORMATTER_HPP
#include <optional>
#include <set>
#include <string>
#include <vector>
#include <mpd/song.h>

#include "ITagged.hpp"

namespace ImpyD::TitleFormatting
{
    std::string FormatITagged(const ITagged &tagged, const std::string &format);

    std::optional<std::string> GetTooFewArgumentsErrorMessage(size_t required, size_t got);

    std::string GetInvalidTagErrorMessage(const std::string &tagName);

    /**
     * Gets all the MPD tag types used in a format string.
     * @param format Format string to collect tags from.
     * @return A set containing all the mpd tags found in the format string
     */
    std::vector<mpd_tag_type> GetUsedTags(const std::string &format);
}

#endif //IM_MPD_TITLEFORMATTER_HPP