#ifndef IM_MPD_MEDIALIBRARYTREE_H
#define IM_MPD_MEDIALIBRARYTREE_H
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
        public:
            std::string content;
            std::unique_ptr<TreeItem> parent;
            std::unique_ptr<std::vector<TreeItem>> children;
            const int layerIndex;
        };

        std::vector<TreeItem> rootItems;

        void FetchChildren(MpdClientWrapper &client, TreeItem &item);

    public:
        IMPYD_REGISTER_PANEL_FactoryFunc(MediaLibraryTree);
        IMPYD_REGISTER_PANEL_GetFactoryName("Media Library Tree");

        explicit MediaLibraryTree(int panelId);

        std::string PanelName() override;

    protected:
        void DrawContents(MpdClientWrapper &client) override;

    public:
        void OnIdleEvent(MpdClientWrapper &client, mpd_idle event) override;

        void InitState(MpdClientWrapper &client) override;
    };
} // ImMPD

#endif //IM_MPD_MEDIALIBRARYTREE_H