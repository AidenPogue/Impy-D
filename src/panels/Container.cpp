//
// Created by aiden on 2025-12-11.
//

#include "Container.hpp"

#include "imgui.h"

ImpyD::Container::Container(int panelId): PanelBase(panelId)
{
}

void ImpyD::Container::DrawContents(Context &context)
{
    //dockspace id can't be 0, and 1 is the main dockspace.
    ImGui::DockSpace(panelId + 100);
}

std::string ImpyD::Container::PanelName()
{
    return GetFactoryName();
}

ImpyD::PanelFlags ImpyD::Container::GetPanelFlags()
{
    return PanelFlags_DrawEarly | PanelFlags_AlwaysDraw;
}

int ImpyD::Container::PushStyleVars()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0,0});
    return 1;
}
