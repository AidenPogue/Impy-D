#include "MediaLibraryTree.hpp"

#include "LibraryLayer.hpp"
#include "../TitleFormatting/TitleFormatter.hpp"

namespace ImpyD {
    //Testing layers!

    static std::vector<LibraryLayer> testLayers =
    {
        LibraryLayer("%albumartist%", ""),
        LibraryLayer("%album% (%date%)", "%date% %album%"),
        // LibraryLayer("%disc%.%track% - %title%", "")
    };

    MediaLibraryTree::TreeItem::TreeItem(TreeItem *parent, std::unique_ptr<TitleFormatting::ITagged> taggedItem,
                                         const int layerIndex, const std::vector<LibraryLayer> &layers) : layerIndex(layerIndex)
    {
        this->parent = parent;
        this->taggedItem = std::move(taggedItem);

        this->content = TitleFormatting::FormatITagged(*taggedItem, layers[layerIndex].displayFormat);
    }

    void MediaLibraryTree::FetchChildren(MpdClientWrapper &client, TreeItem &item)
    {
        if (!item.children)
        {
            item.children = std::make_unique<std::vector<TreeItem>>();
        }
        item.children->clear();

        assert(item.layerIndex <= testLayers.size() - 1);

        auto &childLayer = testLayers[item.layerIndex + 1];
        auto isBaseLayer = item.layerIndex == testLayers.size() - 1;

        client.BeginNoIdle();

        //TESTING!
        auto allTags = TitleFormatting::GetUsedTags(childLayer.displayFormat);
        auto tagVec = std::vector<mpd_tag_type>(allTags.begin(), allTags.end());

        auto listing = client.List(&tagVec);


        client.EndNoIdle();

        for (auto & listItem : listing)
        {
            item.children->emplace_back(TreeItem(&item, std::move(listItem), item.layerIndex + 1, testLayers));
        }
    }

    MediaLibraryTree::MediaLibraryTree(int panelId): PanelBase(panelId)
    {
        rootItems.emplace_back(nullptr, nullptr, -1, testLayers);
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
            FetchChildren(client, rootItems[0]);
        }
    }

    void MediaLibraryTree::InitState(MpdClientWrapper &client)
    {
        FetchChildren(client, rootItems[0]);
    }
} // ImMPD