#include "MainWindow.hpp"

#include "PanelFactory/PanelRegistry.hpp"

namespace ImpyD
{
    static const auto removeClosedPanelPredicate = [](std::unique_ptr<PanelBase>& panel) {return panel->ShouldClose();};

    MainWindow::MainWindow()
    {
        //TESTING! Create all panels.
        for (const auto& pair : PanelFactory::Registry::GetRegisteredPanels())
        {
            panelsToCreate.push_back(pair.first);
        }
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

    void MainWindow::DrawFileMenu()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    //TODO: Centralized exit function that does cleanup/saves configs etc.
                    exit(0);
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void MainWindow::CreatePanelById(MpdClientWrapper &client, const std::string &id)
    {
        auto panel = PanelFactory::Registry::CreatePanelById(id, nextPanelId++);
        panel->InitState(client);

        if (panel->GetPanelFlags() & PanelFlags_DrawEarly)
        {
            panels.push_front(std::move(panel));
        }
        else
        {
            panels.push_back(std::move(panel));
        }
    }

    void MainWindow::Draw(MpdClientWrapper &client)
    {
        // bool idle = false;
        // if (!panelsToCreate.empty())
        // {
        //     client.BeginNoIdle();
        //     idle = true;
        // }
        for (const auto &id : panelsToCreate)
        {
            CreatePanelById(client, id);
        }
        // if (idle)
        // {
        //     client.EndNoIdle();
        // }

        panelsToCreate.clear();

        panels.remove_if(removeClosedPanelPredicate);

        DrawFileMenu();
        DrawLayoutMenu();

        for (const auto &panel : panels)
        {
            panel->Draw(client);
        }
    }

    void MainWindow::SendIdleEventToPanels(MpdClientWrapper &client, mpd_idle event) const
    {
        bool idle = false;
        if (!panels.empty())
        {
            client.BeginNoIdle();
            idle = true;
        }
        for (const auto &panel : panels)
        {
            panel->OnIdleEvent(client, event);
        }
        if (idle)
        {
            client.EndNoIdle();
        }
    }
} // ImpyD