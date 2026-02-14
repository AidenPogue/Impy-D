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

        std::future<std::unique_ptr<MpdSongWrapper>> songFuture;
        std::future<MpdClientWrapper::MpdStatusPtr> statusFuture;

        void GetFutures(Context &context);
        void CheckFutures();

        void SetState(const std::unique_ptr<MpdSongWrapper> &song, const MpdClientWrapper::MpdStatusPtr &status);

    public:
        IMPYD_REGISTER_PANEL_FactoryFunc(SeekBar)
        IMPYD_REGISTER_PANEL_GetFactoryName("Seekbar")

        SeekBar(int panelId)
            : PanelBase(panelId)
        {
        }

        ~SeekBar() override;

        void DrawContents(Context &context) override;

        void OnIdleEvent(Context &context, mpd_idle event) override;

        void InitState(Context &context) override;

        std::string PanelName() override;
    };
}
