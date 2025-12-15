#pragma once

#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD
{
    class SeekBar : public PanelBase, PanelFactory::RegisterPanel<SeekBar>
    {
    private:
        //The glfw time that currentElapsedSettings was last set.
        double elapsedSecondsSetAtTime = 0;
        float currentElapsedSeconds = 0;
        float currentDuration = 0;

        float currentSeek = -1;

        mpd_state currentState = MPD_STATE_STOP;

        void SetState(mpd_song *song, mpd_status *status);

    public:
        IMPYD_REGISTER_PANEL_FactoryFunc(SeekBar)
        IMPYD_REGISTER_PANEL_GetFactoryName("Seekbar")

        SeekBar(int panelId)
            : PanelBase(panelId)
        {
        }

        ~SeekBar() override;

        void DrawContents(MpdClientWrapper &client) override;

        void OnIdleEvent(MpdClientWrapper &client, MpdIdleEventData &data) override;

        void InitState(MpdClientWrapper &client) override;

        const std::string PanelName() override;
    };
}
