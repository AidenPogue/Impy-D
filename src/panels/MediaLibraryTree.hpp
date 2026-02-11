#ifndef IM_MPD_MEDIALIBRARYTREE_H
#define IM_MPD_MEDIALIBRARYTREE_H
#include <set>

#include "LibraryLayer.hpp"
#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD
{
    class MediaLibraryTree : public PanelBase, PanelFactory::RegisterPanel<MediaLibraryTree>
    {
    private:
        class TreeItem
        {

            std::future<std::vector<std::unique_ptr<TitleFormatting::ITagged>>> childrenFuture;
        public:
            TreeItem(TreeItem *parent, std::unique_ptr<TitleFormatting::ITagged> taggedItem, int layerIndex, const std::vector<LibraryLayer> &layers);

            std::string content;
            TreeItem *parent;
            std::unique_ptr<std::vector<TreeItem>> children;
            std::unique_ptr<TitleFormatting::ITagged> taggedItem;
            const int layerIndex;

            std::vector<std::unique_ptr<Mpd::IFilterGenerator>> GetAllFilters();
            void RequestChildren(MpdClientWrapper &client);
            bool WaitingForChildren();
            void ProcessFuture();
        };

        std::vector<TreeItem> rootItems;

    public:
        IMPYD_REGISTER_PANEL_FactoryFunc(MediaLibraryTree);
        IMPYD_REGISTER_PANEL_GetFactoryName("Media Library Tree");

        explicit MediaLibraryTree(int panelId);

        std::string PanelName() override;

    protected:
        void DrawContents(MpdClientWrapper &client) override;

        static void DrawTreeItemContextMenu(MpdClientWrapper &client,
                                            TreeItem &childItem);

        void DrawChildren(MpdClientWrapper &client, TreeItem &item);

    public:
        void OnIdleEvent(MpdClientWrapper &client, mpd_idle event) override;

        void InitState(MpdClientWrapper &client) override;
    };
} // ImMPD

#endif //IM_MPD_MEDIALIBRARYTREE_H