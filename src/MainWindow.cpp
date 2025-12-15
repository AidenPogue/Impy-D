//
// Created by aiden on 2025-12-13.
//

#include "MainWindow.hpp"

#include "PanelFactory/PanelRegistry.hpp"
#include "panels/PlaybackButtons.hpp"

namespace ImpyD
{
    static const auto removeClosedPanelPredicate = [](std::unique_ptr<PanelBase>& panel) {return panel->ShouldClose();};

    MainWindow::MainWindow()
    {

    }

    void MainWindow::DrawLayoutMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("Layout"))
            {
                if (ImGui::BeginMenu("Create Panel"))
                {
                    for (const auto& pair : PanelFactory::Registry::GetRegisteredPanels())
                    {
                        if (ImGui::MenuItem(pair.second.name.c_str()))
                        {
                            panelsToCreate.push_back(pair.first);
                        }
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void MainWindow::CreatePanelById(MpdClientWrapper &client, const std::string &id)
    {
        auto panel = PanelFactory::Registry::CreatePanelById(id, nextPanelId++);
        panel->RegisterCallbacks(client);
        panel->InitState(client);
        panels.push_back(std::move(panel));
    }

    void MainWindow::Draw(MpdClientWrapper &client)
    {
        for (const auto &id : panelsToCreate)
        {
            CreatePanelById(client, id);
        }
        panelsToCreate.clear();

        panels.remove_if(removeClosedPanelPredicate);

        DrawLayoutMenu();

        for (const auto &panel : panels)
        {
            panel->Draw(client);
        }
    }
} // ImpyD