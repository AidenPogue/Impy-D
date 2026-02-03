#include "MediaLibraryTree.hpp"

#include "LibraryLayer.hpp"
#include "../Utils.hpp"
#include "../Mpd/UriFilterGenerator.hpp"
#include "../TitleFormatting/TitleFormatter.hpp"

namespace ImpyD {
    //Testing layers!

    static std::vector<LibraryLayer> testLayers =
    {
        LibraryLayer("%albumartist%", MPD_TAG_ALBUM_ARTIST),
        LibraryLayer("%album% (%date%)", MPD_TAG_DATE),
        LibraryLayer("Disc %disc%", MPD_TAG_DISC, true),
        LibraryLayer("%disc%.%track% - %title% - %artist% (%duration%)", MPD_TAG_TRACK)
    };

    MediaLibraryTree::TreeItem::TreeItem(TreeItem *parent, std::unique_ptr<TitleFormatting::ITagged> taggedItem,
                                         const int layerIndex, const std::vector<LibraryLayer> &layers) : layerIndex(layerIndex)
    {
        this->parent = parent;
        this->taggedItem = std::move(taggedItem);

        if (this->taggedItem)
        {
            this->content = TitleFormatting::FormatITagged(*this->taggedItem, layers[layerIndex].displayFormat);
        }
    }

    std::vector<std::unique_ptr<Mpd::IFilterGenerator>> MediaLibraryTree::TreeItem::GetAllFilters()
    {
        std::vector<std::unique_ptr<Mpd::IFilterGenerator>> allFilters;

        auto curItem = this;

        while (curItem->parent)
        {
            if (curItem->taggedItem)
            {
                auto filters = curItem->taggedItem->GetFilters();

                for (auto &f : filters)
                {
                    allFilters.emplace_back(std::move(f));
                }
            }
            curItem = curItem->parent;
        }

        return allFilters;
    }

    void MediaLibraryTree::FetchChildren(MpdClientWrapper &client, TreeItem &item)
    {
        if (!item.children)
        {
            item.children = std::make_unique<std::vector<TreeItem>>();
        }
        item.children->clear();

        auto childIsBaseLayer = item.layerIndex == testLayers.size() - 2;

        auto allTags = testLayers[item.layerIndex + 1].GetUsedTags();

        auto filters = item.GetAllFilters();

        client.BeginNoIdle();

        //This seems wrong.
        if (childIsBaseLayer)
        {
            auto songs = client.Find(&filters, allTags.front());
            for (auto &song : songs)
            {
                auto copy = std::make_unique<MpdSongWrapper>(song);
                item.children->emplace_back(TreeItem(&item, std::move(copy), item.layerIndex + 1, testLayers));
            }
        }
        else
        {
            auto listing = client.List(&allTags, &filters);
            for (auto &listItem : listing)
            {
                auto copy = std::make_unique<Mpd::ArbitraryTagged>(listItem);
                item.children->emplace_back(TreeItem(&item, std::move(copy), item.layerIndex + 1, testLayers));
            }
        }

        client.EndNoIdle();
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
        static char filter[256];
        static const char *test[] = {"Those", "who", "know"};

        int x = 0;
        ImGui::PushItemWidth(100);
        ImGui::Combo("##layers", &x, test, 3);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
        ImGui::InputTextWithHint("##filter", "Filter...", filter, sizeof(filter));
        ImGui::PopItemWidth();




        if (ImGui::BeginChild("TreeView"))
        {
            DrawChildren(client, rootItems[0]);

            ImGui::EndChild();
        }
    }

    void MediaLibraryTree::DrawTreeItemContextMenu(MpdClientWrapper &client, TreeItem &childItem)
    {
        if (ImGui::BeginPopupContextItem())
        {
            auto append = ImGui::MenuItem("Append to Queue");
            auto send = ImGui::MenuItem("Send to Queue");

            if (append || send)
            {
                client.BeginNoIdle();

                // Only collect all filters if it's not from the base layer (songs just need URL)
                // It would still work fine if we only used GetAllFilters, but it would be redundant for songs.
                auto filters = childItem.layerIndex == testLayers.size() - 1 ? childItem.taggedItem->GetFilters() : childItem.GetAllFilters();

                if (send)
                {
                    client.ClearQueue();
                }

                client.FindAddQueue(&filters);

                if (send)
                {
                    client.PlayCurrent();
                }

                client.EndNoIdle();
            }

            ImGui::EndPopup();
        }
    }

    void MediaLibraryTree::DrawChildren(MpdClientWrapper &client, TreeItem &item)
    {
        if (item.layerIndex == testLayers.size() - 1)
        {
            //Don't fetch children for base layer (because they don't have any)
            return;
        }

        if (!item.children)
        {
            FetchChildren(client, item);
        }

        int flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DrawLinesToNodes;
        //If our children are base layer
        if (item.layerIndex == testLayers.size() - 2)
        {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        for (int i = 0; i < item.children->size(); i++)
        {
            auto &childItem = item.children->at(i);

            ImGui::PushID(i);

            const auto shouldExpandItem = item.children->size() == 1 && testLayers[item.layerIndex + 1].expandIfNoSiblings;

            if (shouldExpandItem)
            {
                DrawChildren(client, childItem);
            }
            else
            {
                auto nodeOpen = ImGui::TreeNodeEx(childItem.content.c_str(), flags);
                //Only draw context menu here because this is the only spot this method actually draws.
                DrawTreeItemContextMenu(client, childItem);
                if (nodeOpen)
                {
                    if (item.children)
                    {
                        DrawChildren(client, childItem);
                    }

                    ImGui::TreePop();
                }
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