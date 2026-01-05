//
// Created by aiden on 2025-12-10.
//

#ifndef IM_MPD_PLAYLISTVIEW_H
#define IM_MPD_PLAYLISTVIEW_H
#include "PanelBase.hpp"
#include "SongTableColumn.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD
{
    class QueueView : public PanelBase, PanelFactory::RegisterPanel<QueueView>
    {
    private:
        std::vector<MpdSongWrapper> currentQueue;
        std::vector<std::vector<std::string>> cellValueCache;
        unsigned currentSongId = 0;

        void UpdateQueue(MpdClientWrapper &client);

        static void CacheRowIfNeeded(MpdSongWrapper &song, const std::vector<SongTableColumn> &columns, std::vector<std::string> &rowCache);

    public:
        IMPYD_REGISTER_PANEL_FactoryFunc(QueueView)
        IMPYD_REGISTER_PANEL_GetFactoryName("Queue View")

        QueueView(int panelId) : PanelBase(panelId) {}

        void DrawContents(MpdClientWrapper &client) override;

        void SetState(MpdClientWrapper &client);

        void OnIdleEvent(MpdClientWrapper &client, mpd_idle event) override;

        void InitState(MpdClientWrapper &client) override;

        std::string PanelName() override;
    };
} // ImMPD

#endif //IM_MPD_PLAYLISTVIEW_H