#include "PanelBase.hpp"

ImpyD::PanelBase::PanelBase(int panelId) : panelId(panelId)
{
}

ImpyD::PanelFlags ImpyD::PanelBase::GetPanelFlags()
{
    return PanelFlags_None;
}

bool ImpyD::PanelBase::ShouldClose() const
{
    return !isOpen;
}

void ImpyD::PanelBase::SetTitle(const std::string &title)
{
    this->title = title + "###" + std::to_string(panelId);
}

void ImpyD::PanelBase::Draw(MpdClientWrapper &client)
{
    //Cant do this in constructor because PanelName is pure virtual
    if (title.empty())
    {
        SetTitle(PanelName());
    }

    if (ImGui::Begin(title.c_str(), &isOpen, windowFlags) || GetPanelFlags() & PanelFlags_AlwaysDraw)
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Change Title"))
            {
            }
            DrawContextMenu(client);
            ImGui::EndPopup();
        }
        DrawContents(client);
    }
    ImGui::End();


}

ImpyD::PanelBase::~PanelBase()
{
    printf("Desctrocota\n");
}

void ImpyD::PanelBase::DrawContextMenu(MpdClientWrapper &client)
{ }
