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
        IMPYD_REGISTER_PANEL_FactoryFunc(Container)
        IMPYD_REGISTER_PANEL_GetFactoryName("Container")

        Container(int panelId) : PanelBase(panelId) {}

        void DrawContents(Context &context) override;

        std::string PanelName() override;

        PanelFlags GetPanelFlags() override;
    };
}

#endif //IM_MPD_CONTAINER_HPP