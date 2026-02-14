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

    void MediaLibraryTree::TreeItem::RequestChildren(Context &context)
    {
        children = nullptr;

        auto childIsBaseLayer = layerIndex == testLayers.size() - 2;

        auto allTags = testLayers[layerIndex + 1].GetUsedTags();

        auto filters = GetAllFilters();

        childrenFuture = childIsBaseLayer ? context.GetClient().Find(std::move(filters), allTags.front()) : context.GetClient().List(allTags, std::move(filters));
    }

    bool MediaLibraryTree::TreeItem::WaitingForChildren()
    {
        return childrenFuture.valid();
    }

    void MediaLibraryTree::TreeItem::ProcessFuture()
    {
        if (Utils::IsReady(childrenFuture))
        {
            if (!children)
            {
                children = std::make_unique<std::vector<TreeItem>>();
            }
            children->clear();

            auto items = childrenFuture.get();

            for (auto &item : items)
            {
                children->emplace_back(TreeItem(this, std::move(item), layerIndex + 1, testLayers));
            }
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

    void MediaLibraryTree::DrawContents(Context &context)
    {
        DrawChildren(context, rootItems[0]);
    }

    void MediaLibraryTree::DrawTreeItemContextMenu(Context &context, TreeItem &childItem)
    {
        if (ImGui::BeginPopupContextItem())
        {
            auto append = ImGui::MenuItem("Append to Queue");
            auto send = ImGui::MenuItem("Replace Queue");

            if (append || send)
            {

                auto &client = context.GetClient();
                // Only collect all filters if it's not from the base layer (songs just need URL)
                // It would still work fine if we only used GetAllFilters, but it would be redundant for songs.
                auto filters = childItem.layerIndex == testLayers.size() - 1 ? childItem.taggedItem->GetFilters() : childItem.GetAllFilters();

                if (send)
                {
                    client.ClearQueue();
                }

                client.FindAddQueue(std::move(filters));

                if (send)
                {
                    client.PlayCurrent();
                }

            }

            ImGui::EndPopup();
        }
    }

    void MediaLibraryTree::DrawChildren(Context &context, TreeItem &item)
    {
        if (item.layerIndex == testLayers.size() - 1)
        {
            //Don't fetch children for base layer (because they don't have any)
            return;
        }

        if (!item.children && !item.WaitingForChildren())
        {
            item.RequestChildren(context);
        }

        int flags = ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DrawLinesToNodes;
        //If our children are base layer
        if (item.layerIndex == testLayers.size() - 2)
        {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        if (!item.WaitingForChildren())
        {
            for (int i = 0; i < item.children->size(); i++)
            {
                auto &childItem = item.children->at(i);

                ImGui::PushID(i);

                const auto shouldExpandItem = item.children->size() == 1 && testLayers[item.layerIndex + 1].expandIfNoSiblings;

                if (shouldExpandItem)
                {
                    DrawChildren(context, childItem);
                }
                else
                {
                    auto nodeOpen = ImGui::TreeNodeEx(childItem.content.c_str(), flags);
                    //Only draw context menu here because this is the only spot this method actually draws.
                    DrawTreeItemContextMenu(context, childItem);
                    if (nodeOpen)
                    {
                        if (item.children)
                        {
                            DrawChildren(context, childItem);
                        }

                        ImGui::TreePop();
                    }
                }

                ImGui::PopID();
            }
        }
        else
        {
            item.ProcessFuture();
            ImGui::Text("Fetching...");
        }
    }

    void MediaLibraryTree::OnIdleEvent(Context &context, mpd_idle event)
    {
        if (event & MPD_IDLE_DATABASE)
        {
            rootItems.front().RequestChildren(context);
        }
    }

    void MediaLibraryTree::InitState(Context &context)
    {
        rootItems.front().RequestChildren(context);
    }
} // ImMPD