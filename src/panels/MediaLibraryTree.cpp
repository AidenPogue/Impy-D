//
// Created by aiden on 2025-12-08.
//

#include "MediaLibraryTree.hpp"

#include "LibraryLayer.hpp"

namespace ImpyD {
    void MediaLibraryTree::FetchRootItems(MpdClientWrapper &client)
    {
        rootItems.clear();
        //Testing layers!

        std::vector<LibraryLayer> layers =
            {
            LibraryLayer("%albumartist%", ""),
            LibraryLayer("%album% (%date%)", "%date% %album%"),
            LibraryLayer("%disc%.%track% - %title%", ""),
        };

        client.BeginNoIdle();
        auto rootList = client.List(MPD_TAG_ALBUM_ARTIST);
        client.EndNoIdle();
        for (const auto &item : rootList)
        {
            rootItems.push_back(TreeItem(item->GetSingleValue(MPD_TAG_ALBUM_ARTIST), nullptr, {}));
        }
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
            if (ImGui::TreeNode(rootItem.content.c_str()))
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
            FetchRootItems(client);
        }
    }

    void MediaLibraryTree::InitState(MpdClientWrapper &client)
    {
        FetchRootItems(client);
    }
} // ImMPD