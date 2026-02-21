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

    bool Config::Library::DrawEditor()
    {
        return false;
    }

    bool Config::Library::LibraryView::DrawEditor()
    {
        return false;
    }
} // ImpyD