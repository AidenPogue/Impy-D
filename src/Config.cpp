#include "Config.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ImpyD
{
    bool Config::Interface::DrawEditor()
    {
        bool edited = false;
        edited |= ImGui::InputText("Window Title Format", &windowTitleFormat);
        return edited;
    }

    std::string Config::Interface::GetDisplayName()
    {
        return "Interface";
    }

    bool Config::Library::DrawEditor()
    {
        return false;
    }

    std::string Config::Library::GetDisplayName()
    {
        return "Library";
    }

    bool Config::Library::LibraryView::DrawEditor()
    {
        return false;
    }

    std::string Config::Library::LibraryView::GetDisplayName()
    {
        return "";
    }
} // ImpyD