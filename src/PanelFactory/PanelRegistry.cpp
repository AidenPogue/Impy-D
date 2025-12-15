//
// Created by aiden on 2025-12-13.
//

#include "PanelRegistry.hpp"

#include <iostream>
#include <stdexcept>

namespace ImpyD::PanelFactory
{
    static std::unordered_map<std::string, RegisteredPanelInfo>& GetPanelMap()
    {
        static std::unordered_map<std::string, RegisteredPanelInfo> registeredPanels;
        return registeredPanels;
    }

    void Registry::RegisterPanel(const RegisteredPanelInfo &info)
    {
        GetPanelMap().emplace(info.id, info);
        //Whose idea was cout bro
        std::cout << "Registered panel " << info.name << " (" << info.id << ")" << std::endl;
    }

    std::unique_ptr<PanelBase> Registry::CreatePanelById(const std::string &panelId, int windowId)
    {
        if (GetPanelMap().find(panelId) == GetPanelMap().end())
        {
            throw std::runtime_error("Panel \"" + panelId + "\" does not exist.");
        }

        return GetPanelMap().at(panelId).factory(windowId);
    }

    const std::unordered_map<std::string, RegisteredPanelInfo>& Registry::GetRegisteredPanels()
    {
        return GetPanelMap();
    }
}


