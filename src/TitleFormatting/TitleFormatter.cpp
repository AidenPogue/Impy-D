#include "TitleFormatter.hpp"

#include <iostream>
#include <regex>

#include "BuiltinFormatFunctions.hpp"
#include "ITagged.hpp"


namespace ImpyD::TitleFormatting
{
    static const auto tagRegex = std::regex("%([a-zA-z]+)%", std::regex_constants::optimize | std::regex_constants::ECMAScript);
    static std::vector<std::string> artistTagListArgs = {"artist", ", "};

    static std::string GetTagValue(const ITagged & tagged, const std::string &tag)
    {
        if (tag == "duration")
        {

        }

        auto tagType = mpd_tag_name_iparse(tag.c_str());
        if (tagType == MPD_TAG_UNKNOWN)
        {
            return GetInvalidTagErrorMessage(tag);
        }

        //List all artists
        if (tagType == MPD_TAG_ARTIST || tagType == MPD_TAG_ARTIST_SORT)
        {
            return BuiltIn::TagList(tagged, artistTagListArgs);
        }

        //Default to first value.
        return tagged.GetSingleValue(tagType);
    }


    std::string FormatITagged(const ITagged &tagged, const std::string &format)
    {
        auto begin = std::sregex_iterator(format.begin(), format.end(), tagRegex);
        auto end = std::sregex_iterator();
        auto outStr = std::string(format);
        int curOffset = 0;
        for (auto it = begin; it != end; ++it)
        {
            auto const &match = *it;
            auto tagValue = GetTagValue(tagged, match.str(1));
            outStr.replace(match.position() + curOffset, match.length(), tagValue);
            curOffset += tagValue.length() - match.length();
        }

        return outStr;
    }

    static std::string GetErrorString(const std::string &message)
    {
        return "<ERROR: " + message + ">";
    }

    std::optional<std::string> GetTooFewArgumentsErrorMessage(size_t required, size_t got)
    {
        if (got < required)
        {
            return GetErrorString("Expected " + std::to_string(required) + " arguments, got " + std::to_string(got));
        }
        return std::nullopt;
    }

    std::string GetInvalidTagErrorMessage(const std::string &tagName)
    {
        return GetErrorString("'" + tagName + "' is not a valid tag");
    }

    std::set<mpd_tag_type> GetUsedTags(const std::string &format)
    {
        std::set<mpd_tag_type> tags;

        auto begin = std::sregex_iterator(format.begin(), format.end(), tagRegex);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it)
        {
            tags.insert(mpd_tag_name_iparse(it->str(1).c_str()));
        }

        return tags;
    }
}


