#ifndef IM_MPD_SETTINGS_HPP
#define IM_MPD_SETTINGS_HPP
#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD::Panels
{
    class Settings : public PanelBase, PanelFactory::RegisterPanel<Settings>
    {
    public:
        explicit Settings(int panelId)
            : PanelBase(panelId)
        {
        }

        IMPYD_REGISTER_PANEL_FactoryFunc(Settings);
        IMPYD_REGISTER_PANEL_GetFactoryName("Settings");

        std::string PanelName() override;

    protected:
        void DrawContents(Context &context) override;
    };
}

#endif
