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
        IMPYD_REGISTER_PANEL_FactoryFunc(VolumeControl)
        IMPYD_REGISTER_PANEL_GetFactoryName("Volume Control Slider")

        VolumeControl(int panelId);

        void DrawContents(MpdClientWrapper &client) override;

        ~VolumeControl() override;

        void OnIdleEvent(MpdClientWrapper &client, mpd_idle event) override;

        void InitState(MpdClientWrapper &client) override;

        std::string PanelName() override;
    };
} // ImMPD

#endif //IM_MPD_VOLUMECONTROL_H