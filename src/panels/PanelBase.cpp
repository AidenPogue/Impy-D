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

static char editingTitle[128];

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
                title.copy(editingTitle, sizeof(editingTitle));
                ImGui::OpenPopup("Rename123");
            }

            ImGui::Separator();

            DrawContextMenu(client);

            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Rename123"))
        {
            ImGui::InputText("New title", editingTitle, sizeof(editingTitle));
            if (ImGui::Button("Rename"))
            {
                SetTitle(editingTitle);
                ImGui::CloseCurrentPopup();
            }

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
