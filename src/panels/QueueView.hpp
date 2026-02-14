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
        std::future<std::vector<std::unique_ptr<TitleFormatting::ITagged>>> queueFuture;

        std::vector<std::unique_ptr<TitleFormatting::ITagged>> currentQueue;
        std::vector<std::vector<std::string>> cellValueCache;
        unsigned currentSongId = 0;

        void UpdateQueue(std::vector<std::unique_ptr<TitleFormatting::ITagged>> songs);

        static void CacheRowIfNeeded(std::unique_ptr<TitleFormatting::ITagged> &song, const std::vector<SongTableColumn> &columns, std::vector<std::string> &rowCache);

    public:
        IMPYD_REGISTER_PANEL_FactoryFunc(QueueView)
        IMPYD_REGISTER_PANEL_GetFactoryName("Queue View")

        QueueView(int panelId) : PanelBase(panelId) {}

        void DrawContents(Context &context) override;

        void SetState(Context &context);

        void OnIdleEvent(Context &context, mpd_idle event) override;

        void InitState(Context &context) override;

        std::string PanelName() override;
    };
} // ImMPD

#endif //IM_MPD_PLAYLISTVIEW_H