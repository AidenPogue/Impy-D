//
// Created by aiden on 2025-12-11.
//

#ifndef IM_MPD_CONTAINER_HPP
#define IM_MPD_CONTAINER_HPP
#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"


namespace ImpyD
{
    class Container : public PanelBase, PanelFactory::RegisterPanel<Container>
    {
    public:
        IMPYD_REGISTER_PANEL_FUNCS(Container, IMPYD_BUILTIN_ID_PREFIX "container", "Container");

        void DrawContents(Context &context) override;

        std::string PanelName() override;

        PanelFlags GetPanelFlags() override;

    protected:
        int PushStyleVars() override;
    };
}

#endif //IM_MPD_CONTAINER_HPP