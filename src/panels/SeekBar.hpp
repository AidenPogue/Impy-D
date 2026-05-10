#pragma once

#include "PanelBase.hpp"
#include "../Utils.hpp"
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
        std::string currentDurationString;
        float currentSeek = -1;

        //Show remaining time instead of elapsed time.
        bool showRemainingTime = false;

        mpd_state currentState = MPD_STATE_STOP;

        std::future<std::unique_ptr<MpdSongWrapper>> songFuture;
        std::future<MpdClientWrapper::MpdStatusPtr> statusFuture;



        void GetFutures(Context &context);
        void CheckFutures();

        void SetState(const std::unique_ptr<MpdSongWrapper> &song, const MpdClientWrapper::MpdStatusPtr &status);

    public:
        IMPYD_REGISTER_PANEL_FUNCS(SeekBar, IMPYD_BUILTIN_ID_PREFIX "seekbar", "Seekbar");

        explicit SeekBar(int panelId)
            : PanelBase(panelId), currentDurationString(Utils::SecondsToDurationString(0))
        {
        }

        ~SeekBar() override;

        void DrawContents(Context &context) override;

        void OnIdleEvent(Context &context, mpd_idle event) override;

        void InitState(Context &context) override;

        std::string PanelName() override;

        void DrawContextMenu(Context &context) override;
    };
}
