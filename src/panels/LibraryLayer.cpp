#include "LibraryLayer.hpp"

#include "imgui.h"
#include <misc/cpp/imgui_stdlib.h>
#include "../Utils.hpp"
#include "../TitleFormatting/TitleFormatter.hpp"

ImpyD::LibraryLayer::LibraryLayer(std::string display_format, mpd_tag_type sortingTag, bool expand_if_no_siblings)
      : displayFormat(std::move(display_format)),
      sortingTag(sortingTag),
      expandIfNoSiblings(expand_if_no_siblings)
{
}

std::vector<mpd_tag_type> ImpyD::LibraryLayer::GetUsedTags() const
{
      auto tags = std::vector<mpd_tag_type>();
      tags.push_back(sortingTag);
      auto fmtTags = ImpyD::TitleFormatting::GetUsedTags(displayFormat);
      ImpyD::Utils::TagSetUnion(tags, fmtTags);
      return tags;
}

std::string ImpyD::LibraryLayer::GetDisplayName()
{
}

bool ImpyD::LibraryLayer::DrawEditor()
{
      auto edited = false;
      edited |= ImGui::InputText("Pattern", &displayFormat);
      edited |= ImGui::Checkbox("Expand If No Siblings", &expandIfNoSiblings);
      return edited;
}
