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

void ImpyD::PlaybackButtonsPanel::DrawContents(Context &context)
{
    auto &client = context.GetClient();

    if (ImGui::Button("|<<"))
    {
        client.Prev();
    }

    ImGui::SameLine();

    if (ImGui::Button(">||"))
    {
        client.Toggle();
    }

    ImGui::SameLine();

    if (ImGui::Button(">>|"))
    {
        client.Next();
    }
}

std::string ImpyD::PlaybackButtonsPanel::PanelName()
{
    return GetFactoryName();
}
