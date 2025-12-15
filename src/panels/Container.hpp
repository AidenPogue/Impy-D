//
// Created by aiden on 2025-12-11.
//

#ifndef IM_MPD_CONTAINER_HPP
#define IM_MPD_CONTAINER_HPP
#include "PanelBase.hpp"


namespace ImpyD
{
    class Container : public PanelBase
    {
    public:
        Container(int panelId) : PanelBase(panelId) {}

        void DrawContents(MpdClientWrapper &client) override;
    };
}

#endif //IM_MPD_CONTAINER_HPP