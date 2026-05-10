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
            void RequestChildren(Context &context);
            bool WaitingForChildren();
            void ProcessFuture(Context &context);
        };

        std::vector<TreeItem> rootItems;

    public:
        IMPYD_REGISTER_PANEL_FUNCS(MediaLibraryTree, IMPYD_BUILTIN_ID_PREFIX "library-tree", "Media Library Tree");

        explicit MediaLibraryTree(int panelId);

        std::string PanelName() override;

    protected:
        void DrawContents(Context &context) override;

        static void DrawTreeItemContextMenu(Context &context,
                                            TreeItem &childItem);

        void DrawChildren(Context &context, TreeItem &item);

    public:
        void OnIdleEvent(Context &context, mpd_idle event) override;

        void InitState(Context &context) override;
    };
} // ImMPD

#endif //IM_MPD_MEDIALIBRARYTREE_H