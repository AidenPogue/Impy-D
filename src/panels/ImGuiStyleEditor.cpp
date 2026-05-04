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

    ImGuiStyleEditor::ImGuiStyleEditor(int panelId) : PanelBase(panelId)
    {
    }

    std::string ImGuiStyleEditor::PanelName()
    {
        return GetFactoryName();
    }
} // ImpyD