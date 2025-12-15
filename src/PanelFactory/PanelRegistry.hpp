#ifndef IM_MPD_PANELREGISTRY_HPP
#define IM_MPD_PANELREGISTRY_HPP
#include <memory>
#include <string>
#include <utility>

#include "RegisteredPanelInfo.hpp"
#include "../panels/PanelBase.hpp"

namespace ImpyD::PanelFactory
{
    class Registry
    {
    public:
        Registry() = delete;

        static void RegisterPanel(const RegisteredPanelInfo &info);
        static std::unique_ptr<PanelBase> CreatePanelById(const std::string &panelId, int windowId);
        static const std::unordered_map<std::string, RegisteredPanelInfo>& GetRegisteredPanels();
    };

}

#endif //IM_MPD_PANELREGISTRY_HPP