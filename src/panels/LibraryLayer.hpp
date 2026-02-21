#ifndef IM_MPD_LIBRARYLAYER_HPP
#define IM_MPD_LIBRARYLAYER_HPP
#include <set>
#include <string>
#include <vector>
#include <mpd/tag.h>

#include <nlohmann/json.hpp>

namespace ImpyD
{
    class LibraryLayer
    {
    public:
        LibraryLayer() = default;
        LibraryLayer(std::string display_format, mpd_tag_type sortingTag, bool expand_if_no_siblings = false);

        std::string displayFormat;
        mpd_tag_type sortingTag = MPD_TAG_UNKNOWN;
        /**
         * If true, any children of an instance of this layer will be placed into the instance's parent if it is the only child of the parent.
         */
        bool expandIfNoSiblings = false;

        /**
         * Gets a vector of all the unique tags used in this layer's format, with sortingTag always being the first element.
         * @return A vector of the used tags
         */
        [[nodiscard]] std::vector<mpd_tag_type> GetUsedTags() const;
    };

    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE_WITH_DEFAULT(LibraryLayer, displayFormat, sortingTag, expandIfNoSiblings)
}

#endif //IM_MPD_LIBRARYLAYER_HPP