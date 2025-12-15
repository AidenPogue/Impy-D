#include "PlaybackButtons.hpp"
#include "imgui.h"
#include <vector>

struct PlaybackButton
{
    void (*onClick) (MpdClientWrapper *client);
    const char *text;
    const char *name;
    bool enabled = false;

    PlaybackButton(void (*onClick) (MpdClientWrapper *client), const char *text, const char *name)
    {
        this->onClick = onClick;
        this->text = text;
        this->name = name;
    }
};

ImpyD::PlaybackButtonsPanel::~PlaybackButtonsPanel() = default;

void ImpyD::PlaybackButtonsPanel::DrawContents(MpdClientWrapper &client)
{
    if (ImGui::BeginPopupContextWindow())
    {
        // ImGui::Selectable("Test", &test);
        ImGui::EndPopup();
    }

    if (ImGui::Button("|<<")) {
        client.BeginNoIdle();
        client.Prev();
        client.EndNoIdle();
    }

    ImGui::SameLine();

    if (ImGui::Button(">||")) {
        client.BeginNoIdle();
        client.Toggle();
        client.EndNoIdle();
    }
    ImGui::SameLine();

    if (ImGui::Button(">>|")) {
        client.BeginNoIdle();
        client.Next();
        client.EndNoIdle();
    }
}

const std::string ImpyD::PlaybackButtonsPanel::PanelName()
{
    return GetFactoryName();
}
