#ifndef IM_MPD_VOLUMEMETER_HPP
#define IM_MPD_VOLUMEMETER_HPP
#include <fstream>

#include "PanelBase.hpp"
#include "PeakMeterBar.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD
{
    class VolumeMeter : public PanelBase, PanelFactory::RegisterPanel<VolumeMeter>
    {
        PeakMeterBar leftBar, rightBar;

    public:
        explicit VolumeMeter(int panelId)
            : PanelBase(panelId)
        {
        }

        IMPYD_REGISTER_PANEL_FactoryFunc(VolumeMeter);
        IMPYD_REGISTER_PANEL_GetFactoryName("Volume Meter");

        std::string PanelName() override;

    protected:
        void DrawContents(Context &context) override;

    public:
        void InitState(Context &context) override;
    };
}



#endif //IM_MPD_VOLUMEMETER_HPP