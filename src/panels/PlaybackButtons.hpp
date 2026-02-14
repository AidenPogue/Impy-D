#pragma once

#include "PanelBase.hpp"
#include "../Mpd/MpdClientWrapper.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD
{
    class PlaybackButtonsPanel : public PanelBase, PanelFactory::RegisterPanel<PlaybackButtonsPanel>
    {
    private:
    public:
        IMPYD_REGISTER_PANEL_FactoryFunc(PlaybackButtonsPanel);
        IMPYD_REGISTER_PANEL_GetFactoryName("Playback Control Buttons")

        PlaybackButtonsPanel(int panelId) : PanelBase(panelId) {windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;}

        ~PlaybackButtonsPanel() override;

        void DrawContents(Context &context) override;

        std::string PanelName() override;
    };
}
