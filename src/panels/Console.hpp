#ifndef IM_MPD_CONSOLE_HPP
#define IM_MPD_CONSOLE_HPP
#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD
{
    class Console : public PanelBase, PanelFactory::RegisterPanel<Console>
    {
        std::string buffer;

    public:
        explicit Console(int panelId)
            : PanelBase(panelId)
        {
        }

        IMPYD_REGISTER_PANEL_FactoryFunc(Console);
        IMPYD_REGISTER_PANEL_GetFactoryName("Console");

        std::string PanelName() override;

    protected:
        void DrawContents(Context &context) override;
    };
} // ImpyD

#endif //IM_MPD_CONSOLE_HPP