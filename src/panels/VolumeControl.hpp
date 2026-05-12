//
// Created by aiden on 2025-12-09.
//

#ifndef IM_MPD_VOLUMECONTROL_H
#define IM_MPD_VOLUMECONTROL_H
#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD {
    class VolumeControl : public PanelBase, PanelFactory::RegisterPanel<VolumeControl>
    {
        std::future<MpdClientWrapper::MpdStatusPtr> statusFuture;

        int editingValue = -1;
        int currentValue = 0;
        void SetState(const MpdClientWrapper::MpdStatusPtr &status);

    public:
        VolumeControl()
        {
            windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        }

        IMPYD_REGISTER_PANEL_FUNCS(VolumeControl, IMPYD_BUILTIN_ID_PREFIX "volume-control", "Volume Control Slider");

        void DrawContents(Context &context) override;

        ~VolumeControl() override;

        void OnIdleEvent(Context &context, mpd_idle event) override;

        void InitState(Context &context) override;

        std::string PanelName() override;
    };
} // ImMPD

#endif //IM_MPD_VOLUMECONTROL_H