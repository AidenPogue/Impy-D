#include "Config.hpp"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

namespace ImpyD
{
    bool Config::Interface::DrawEditor()
    {
        bool edited = false;
        edited |= ImGui::InputText("Window Title Format", &windowTitleFormat);
        return edited;
    }

    std::string Config::Interface::GetDisplayName()
    {
        return "Interface";
    }

    bool Config::Library::DrawEditor()
    {
        auto edited = false;
        if (ImGui::CollapsingHeader("Library Views"))
        {
            if (ImGui::BeginListBox("##views"))
            {
                for (int i = 0; i < libraryViews.size(); ++i)
                {
                    ImGui::BeginGroup();
                    ImGui::PushID(i);
                    edited |= libraryViews[i].DrawEditor();
                    ImGui::PopID();
                    ImGui::EndGroup();
                }
                ImGui::EndListBox();
            }
        }

        return edited;
    }

    std::string Config::Library::GetDisplayName()
    {
        return "Library";
    }

    bool Config::Library::LibraryView::DrawEditor()
    {
        auto edited = false;
        edited |= ImGui::InputText("Name", &name);

        if (ImGui::BeginListBox("##layers", ImGui::GetContentRegionAvail()))
        {
            for (int i = 0; i < layers.size(); ++i)
            {
                ImGui::BeginGroup();
                ImGui::PushID(i);
                edited |= layers[i].DrawEditor();
                ImGui::PopID();
                ImGui::EndGroup();
            }

            ImGui::EndListBox();
        }


        return edited;
    }

    std::string Config::Library::LibraryView::GetDisplayName()
    {
        return "";
    }

    std::string Config::Visualizer::GetDisplayName()
    {
        return "Visualizer";
    }

    bool Config::Visualizer::DrawEditor()
    {
        auto edited = false;
        edited |= ImGui::InputText("PCM Stream Path", &fifoPath);
        edited |= ImGui::InputInt("Sample Rate", &fifoSampleRate);
        edited |= ImGui::InputInt("Bit Depth", &fifoDepth, 8);
        edited |= ImGui::InputInt("Channels", &fifoChannels);
        return edited;
    }
} // ImpyD