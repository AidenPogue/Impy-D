#include "Settings.hpp"

#include <iostream>

namespace ImpyD::Panels
{
    std::string Settings::PanelName()
    {
        return GetFactoryName();
    }

    void Settings::DrawContents(Context &context)
    {
        static bool edited;

        IConfigItem *categories[] = {&context.GetConfig().interface, &context.GetConfig().library, &context.GetConfig().visualizer};
        if (!selectedTopLevelCategory)
        {
            selectedTopLevelCategory = categories[0];
        }

        if (ImGui::BeginTable("##table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_Resizable))
        {
            ImGui::TableNextColumn();

            auto listSize = ImGui::GetContentRegionAvail();
            listSize.y -= ImGui::GetTextLineHeight();
            if (ImGui::BeginListBox("##cat", listSize))
            {
                for (auto category : categories)
                {
                    auto selected = selectedTopLevelCategory == category;
                    auto name = category->GetDisplayName();
                    ImGui::Selectable(name.c_str(), &selected);
                    if (selected)
                    {
                        selectedTopLevelCategory = category;
                    }
                }
                ImGui::EndListBox();
            }

            ImGui::BeginDisabled(!edited);
            if (ImGui::Button("Save Config"))
            {
                SaveConfig(context.GetConfig());
                edited = false;
            }
            ImGui::EndDisabled();

            ImGui::TableNextColumn();

            if (ImGui::BeginChild("##editor", ImGui::GetContentRegionAvail()))
            {
                edited |= selectedTopLevelCategory->DrawEditor();
                ImGui::EndChild();
            }

            ImGui::EndTable();
        }
    }
}
