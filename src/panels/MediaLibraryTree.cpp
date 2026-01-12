#include "MediaLibraryTree.hpp"

#include "LibraryLayer.hpp"
#include "../TitleFormatting/TitleFormatter.hpp"

namespace ImpyD {
    //Testing layers!

    static std::vector<LibraryLayer> testLayers =
    {
        LibraryLayer("%albumartist%", ""),
        LibraryLayer("%album% (%date%)", "%date% %album%")
    };

    static auto testBaseLayer = LibraryLayer("%disc%.%track% - %title%", "");

    void MediaLibraryTree::FetchChildren(MpdClientWrapper &client, TreeItem &item)
    {
        if (!item.children)
        {
            item.children = std::make_unique<std::vector<TreeItem>>();
        }
        item.children->clear();

        auto isBaseLayer = item.layerIndex == testLayers.size();
        auto &layer = isBaseLayer ? testBaseLayer : testLayers[item.layerIndex + 1];

        client.BeginNoIdle();

        if (isBaseLayer)
        {

        }
        else
        {
            item.children->push_back(client.List())
        }

        client.EndNoIdle();

        for (const auto &item : rootList)
        {
            item.push_back(TreeItem(TitleFormatting::FormatITagged(*item, layers.front().displayFormat), nullptr, nullptr));
        }
    }

    MediaLibraryTree::MediaLibraryTree(int panelId): PanelBase(panelId)
    {
        rootItems.push_back(TreeItem("Music Library", nullptr, nullptr, -1));
    }

    std::string MediaLibraryTree::PanelName()
    {
        return GetFactoryName();
    }

    void MediaLibraryTree::DrawContents(MpdClientWrapper &client)
    {
        for (int i = 0; i < rootItems.size(); ++i)
        {
            const auto &rootItem = rootItems[i];

            ImGui::PushID(i);
            if (ImGui::TreeNodeEx(rootItem.content.c_str(), ImGuiTreeNodeFlags_SpanFullWidth))
            {
                ImGui::TreePop();
            }
            ImGui::PopID();
        }

    }

    void MediaLibraryTree::OnIdleEvent(MpdClientWrapper &client, mpd_idle event)
    {
        if (event & MPD_IDLE_DATABASE)
        {
            FetchChildren(client, TODO);
        }
    }

    void MediaLibraryTree::InitState(MpdClientWrapper &client)
    {
        FetchChildren(client, TODO);
    }
} // ImMPD