#ifndef IM_MPD_LIBRARYLAYER_HPP
#define IM_MPD_LIBRARYLAYER_HPP
#include <set>
#include <string>
#include <vector>
#include <mpd/tag.h>


class LibraryLayer
{
public:
    LibraryLayer(std::string display_format, std::string sorting_format, bool eager_load_children = false,
        bool expand_if_no_siblings = false);

    const std::string displayFormat;
    const std::string sortingFormat;
    const bool eagerLoadChildren = false;
    /**
     * If true, any children of an instance of this layer will be placed into the instance's parent if it is the only child of the parent.
     */
    const bool expandIfNoSiblings = false;

    const std::vector<mpd_tag_type> usedTags;

    [[nodiscard]] std::vector<mpd_tag_type> GetUsedTags() const;
};


#endif //IM_MPD_LIBRARYLAYER_HPP