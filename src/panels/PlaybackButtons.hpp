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
        IMPYD_REGISTER_PANEL_FUNCS(PlaybackButtonsPanel, IMPYD_BUILTIN_ID_PREFIX "playback-buttons", "Playback Control Buttons");

        explicit PlaybackButtonsPanel(int panelId) : PanelBase(panelId) {windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;}

        ~PlaybackButtonsPanel() override;

        void DrawContents(Context &context) override;

        std::string PanelName() override;
    };
}
