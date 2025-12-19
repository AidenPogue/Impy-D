#ifndef IM_MPD_LIBRARYLAYER_HPP
#define IM_MPD_LIBRARYLAYER_HPP
#include <string>


class LibraryLayer
{
public:
    LibraryLayer(std::string display_format, std::string sorting_format, bool eager_load_children = false,
        bool expand_if_no_siblings = false)
        : displayFormat(std::move(display_format)),
          sortingFormat(std::move(sorting_format)),
          eagerLoadChildren(eager_load_children),
          expandIfNoSiblings(expand_if_no_siblings)
    {
    }

private:
    std::string displayFormat;
    std::string sortingFormat;
    bool eagerLoadChildren = false;
    /**
     * If true, any children of an instance of this layer will be placed into the instance's parent if it is the only child of the parent.
     */
    bool expandIfNoSiblings = false;
};


#endif //IM_MPD_LIBRARYLAYER_HPP