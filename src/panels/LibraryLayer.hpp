#ifndef IM_MPD_LIBRARYLAYER_HPP
#define IM_MPD_LIBRARYLAYER_HPP
#include <set>
#include <string>
#include <vector>
#include <mpd/tag.h>


class LibraryLayer
{
public:
    LibraryLayer(std::string display_format, mpd_tag_type sortingTag, bool expand_if_no_siblings = false);

    const std::string displayFormat;
    const mpd_tag_type sortingTag;
    /**
     * If true, any children of an instance of this layer will be placed into the instance's parent if it is the only child of the parent.
     */
    const bool expandIfNoSiblings = false;

    const std::vector<mpd_tag_type> usedTags;

    /**
     * Gets a vector of all the unique tags used in this layer's format, with sortingTag always being the first element.
     * @return A vector of the used tags
     */
    [[nodiscard]] std::vector<mpd_tag_type> GetUsedTags() const;
};


#endif //IM_MPD_LIBRARYLAYER_HPP