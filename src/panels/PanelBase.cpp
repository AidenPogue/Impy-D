#include "PanelBase.hpp"

ImpyD::PanelBase::PanelBase(int panelId) : panelId(panelId)
{
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

    if (ImGui::Begin(title.c_str(), &isOpen, windowFlags))
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Change Title"))
            {
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

void ImpyD::PanelBase::RegisterCallbacks(MpdClientWrapper &client)
{
    callbackId = client.AddIdleListener([this](MpdClientWrapper &c, MpdIdleEventData &d) {this->OnIdleEvent(c, d);});
}
