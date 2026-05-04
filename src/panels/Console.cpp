#include "Console.hpp"
#include "misc/cpp/imgui_stdlib.h"
#include <iostream>

namespace ImpyD
{
    std::string Console::PanelName()
    {
        return GetFactoryName();
    }

    void Console::DrawContents(Context &context)
    {
        ImGui::InputTextMultiline("##console", &buffer, ImGui::GetContentRegionAvail(), ImGuiInputTextFlags_ReadOnly);
    }
} // ImpyD