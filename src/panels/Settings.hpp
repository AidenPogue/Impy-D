#ifndef IM_MPD_SETTINGS_HPP
#define IM_MPD_SETTINGS_HPP
#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD::Panels
{
    class Settings : public PanelBase, PanelFactory::RegisterPanel<Settings>
    {
        IConfigItem *selectedTopLevelCategory = nullptr;

    public:

        IMPYD_REGISTER_PANEL_FUNCS(Settings, IMPYD_BUILTIN_ID_PREFIX "settings", "Settings");
        std::string PanelName() override;

    protected:
        void DrawContents(Context &context) override;
    };
}

#endif
