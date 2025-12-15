#ifndef IM_MPD_REGISTEREDPANELINFO_HPP
#define IM_MPD_REGISTEREDPANELINFO_HPP
#include <memory>
#include <string>

#include "PanelFactoryFunc.hpp"

namespace ImpyD::PanelFactory
{
    class RegisteredPanelInfo
    {
    public:
        RegisteredPanelInfo(std::string id, std::string name, PanelFactoryFunc factory)
            : id(std::move(id)),
              name(std::move(name)),
              factory(factory)
        {
        }

        const std::string id;
        const std::string name;
        const PanelFactoryFunc factory;
    };
}

#endif //IM_MPD_REGISTEREDPANELINFO_HPP