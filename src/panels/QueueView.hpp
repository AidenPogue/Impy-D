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
        std::vector<mpd_song *> currentQueue;
        std::vector<std::vector<std::string>> cellValueCache;
        unsigned currentSongId = 0;

        void UpdateQueue(MpdClientWrapper *client);

        static void CacheRowIfNeeded(mpd_song *song, const std::vector<SongTableColumn> &columns, std::vector<std::string> &rowCache);

    public:
        IMPYD_REGISTER_PANEL_FactoryFunc(QueueView)
        IMPYD_REGISTER_PANEL_GetFactoryName("Queue View")

        QueueView(int panelId) : PanelBase(panelId) {}

        void DrawContents(MpdClientWrapper &client) override;

        void OnIdleEvent(MpdClientWrapper &client, MpdIdleEventData &data) override;

        void InitState(MpdClientWrapper &client) override;

        const std::string PanelName() override;
    };
} // ImMPD

#endif //IM_MPD_PLAYLISTVIEW_H