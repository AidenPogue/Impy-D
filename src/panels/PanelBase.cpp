#include "PanelBase.hpp"
#include "misc/cpp/imgui_stdlib.h"

#define CHANGE_TITLE_MODAL_ID "Change Title"
#define OPTIONS_MODAL_ID "Options"

void ImpyD::PanelBase::Initialize(int panelId)
{
    if (panelId < 0)
    {
        throw std::invalid_argument("panelId cannot be negative");
    }
    if (this->panelId > 0)
    {
        throw std::runtime_error("Panel is already initialized with id " + std::to_string(this->panelId));
    }

    this->panelId = panelId;

    //Make sure titleWithId is set
    SetTitle(title);
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
    this->titleWithId = title + "###" + std::to_string(panelId);
    this->title = std::string(title);
}

void ImpyD::PanelBase::Draw(Context &context)
{
    //Cant do this in constructor because PanelName is pure virtual
    if (titleWithId.empty())
    {
        SetTitle(PanelName());
    }

    auto styleVarCount = PushStyleVars();
    auto windowOpen = ImGui::Begin(titleWithId.c_str(), &isOpen, windowFlags) || GetPanelFlags() & PanelFlags_AlwaysDraw;
    ImGui::PopStyleVar(styleVarCount);

    if (windowOpen)
    {
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Change Title"))
            {
                shouldOpenChangeTitleModal = true;
            }

            ImGui::Separator();

            DrawContextMenu(context);
            ImGui::EndPopup();
        }
        DrawContents(context);
    }

    static std::string editingTitle;

    if (shouldOpenChangeTitleModal)
    {
        shouldOpenChangeTitleModal = false;
        editingTitle = title;
        ImGui::OpenPopup(CHANGE_TITLE_MODAL_ID);
    }

    if (ImGui::BeginPopupModal(CHANGE_TITLE_MODAL_ID, nullptr, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::SetItemDefaultFocus();
        if (ImGui::InputText("Title", &editingTitle))
        {
        }

        if (ImGui::Button("Ok"))
        {
            SetTitle(editingTitle);
            ImGui::CloseCurrentPopup();
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
        {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();


}

ImpyD::PanelBase::~PanelBase()
{
}

void ImpyD::PanelBase::DrawContextMenu(Context &context)
{ }
