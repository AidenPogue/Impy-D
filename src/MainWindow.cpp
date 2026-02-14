#include "MainWindow.hpp"

#include "imgui_internal.h"
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

    void MainWindow::CreatePanelById(Context &context, const std::string &id)
    {
        auto panel = PanelFactory::Registry::CreatePanelById(id, nextPanelId++);
        panel->InitState(context);

        if (panel->GetPanelFlags() & PanelFlags_DrawEarly)
        {
            panels.push_front(std::move(panel));
        }
        else
        {
            panels.push_back(std::move(panel));
        }
    }

    void MainWindow::Draw(Context &context)
    {
        for (const auto &id : panelsToCreate)
        {
            CreatePanelById(context, id);
        }

        panelsToCreate.clear();

        panels.remove_if(removeClosedPanelPredicate);

        DrawFileMenu();
        DrawLayoutMenu();

        if (!context.GetClient().GetIsConnected())
        {
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2, 2));
            if (ImGui::BeginViewportSideBar("SatusBar", ImGui::GetWindowViewport(), ImGuiDir_Down, 24,  ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar))
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
                ImGui::Text("Could not connect to MPD!");
                ImGui::PopStyleColor();
                ImGui::End();
            }
            ImGui::PopStyleVar();

        }

        for (const auto &panel : panels)
        {
            panel->Draw(context);
        }
    }

    void MainWindow::SendIdleEventToPanels(Context &context, mpd_idle event) const
    {
        for (const auto &panel : panels)
        {
            panel->OnIdleEvent(context, event);
        }
    }
} // ImpyD