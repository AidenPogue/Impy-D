#include "LibraryLayer.hpp"

#include "../TitleFormatting/TitleFormatter.hpp"

LibraryLayer::LibraryLayer(std::string display_format, std::string sorting_format, bool eager_load_children, bool expand_if_no_siblings)
      : displayFormat(std::move(display_format)),
      sortingFormat(std::move(sorting_format)),
      eagerLoadChildren(eager_load_children),
      expandIfNoSiblings(expand_if_no_siblings)
{
      //TODO: include sorting format tags too.
}

std::vector<mpd_tag_type> LibraryLayer::GetUsedTags() const
{
      return ImpyD::TitleFormatting::GetUsedTags(displayFormat);
}
