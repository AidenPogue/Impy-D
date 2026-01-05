//
// Created by aiden on 2025-12-16.
//

#ifndef IM_MPD_ITAGGED_HPP
#define IM_MPD_ITAGGED_HPP
#include <memory>
#include <vector>
#include <mpd/tag.h>

namespace ImpyD::TitleFormatting
{
    /**
     * Object with an arbitrary number of key value pair metadata tags
     */
    class ITagged
    {
    public:
        virtual ~ITagged() = default;

        [[nodiscard]] virtual std::string GetSingleValue(mpd_tag_type key) const = 0;
        [[nodiscard]] virtual std::vector<std::string> GetAllValues(mpd_tag_type key) const = 0;

        [[nodiscard]] virtual unsigned GetId() const;
        [[nodiscard]] virtual unsigned GetDurationMs() const;
        [[nodiscard]] virtual std::string GetUri() const;
    };
}

#endif //IM_MPD_ITAGGED_HPP