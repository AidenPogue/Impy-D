//
// Created by aiden on 2026-01-17.
//

#include "ImGuiStyleEditor.hpp"

namespace ImpyD
{
    void ImGuiStyleEditor::DrawContents(Context &context)
    {
        ImGui::ShowStyleEditor();
    }

    std::string ImGuiStyleEditor::PanelName()
    {
        return GetFactoryName();
    }
} // ImpyD