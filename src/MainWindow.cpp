#include "MainWindow.hpp"

#include <GLFW/glfw3.h>

#include "imgui_internal.h"
#include "Utils.hpp"
#include "PanelFactory/PanelRegistry.hpp"
#include "TitleFormatting/TitleFormatter.hpp"

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

    void MainWindow::Initialize(Context &context)
    {
        RequestTitleUpdate(context);
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

    void MainWindow::AddPanel(Context &context, std::unique_ptr<PanelBase> &panel)
    {
        if (!panel->GetIsIdValid())
        {
            throw std::runtime_error("Panel does not have a valid ID.");
        }

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

    void MainWindow::CreatePanelById(Context &context, const std::string &id)
    {
        auto panel = PanelFactory::Registry::CreatePanelById(id);

        panel->SetId(nextPanelId++);

        AddPanel(context, panel);
    }

    void MainWindow::SetWindowTitle(Context &context)
    {
        auto s = songFuture.get();
        if (!s)
        {
            return;
        }

        glfwSetWindowTitle(glfwGetCurrentContext(), TitleFormatting::FormatITagged(*s, context.GetConfig().interface.windowTitleFormat).c_str());
    }

    void MainWindow::Draw(Context &context)
    {
        if (Utils::IsReady(songFuture))
        {
            SetWindowTitle(context);
        }

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

    void MainWindow::RequestTitleUpdate(Context &context)
    {
        songFuture = context.GetClient().GetCurrentSong();
    }

    void MainWindow::SendIdleEventToPanels(Context &context, mpd_idle event)
    {
        if (event & MPD_IDLE_PLAYER)
        {
            RequestTitleUpdate(context);
        }

        for (const auto &panel : panels)
        {
            panel->OnIdleEvent(context, event);
        }
    }
} // ImpyD