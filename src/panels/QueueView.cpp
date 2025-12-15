//
// Created by aiden on 2025-12-10.
//

#include "QueueView.hpp"

#include <iostream>

#include "imgui.h"
#include "SongTableColumn.hpp"
#include "../Utils.hpp"
#include "../TitleFormatting/TitleFormatter.hpp"

namespace ImpyD
{
    static std::vector<SongTableColumn> testColumns =
        {
        SongTableColumn("disc-track", "Disc + Track", "%disc%.%track%"),
        SongTableColumn("title", "Title", "%title%"),
        SongTableColumn("artist", "Artist", "%artist%"),
        SongTableColumn("album", "Album", "%album%"),
    };

    void QueueView::UpdateQueue(MpdClientWrapper *client)
    {
        Utils::FreeSongList(currentQueue);
        client->GetQueue(currentQueue);
        cellValueCache.resize(currentQueue.size());
        for (auto &row : cellValueCache)
        {
            row.clear();
        }
    }

    void QueueView::CacheRowIfNeeded(mpd_song *song, const std::vector<SongTableColumn> &columns,
        std::vector<std::string> &rowCache)
    {
        if (rowCache.size() == columns.size())
        {
           return;
        }

        rowCache.clear();

        for (const auto &column : columns)
        {
            rowCache.push_back(TitleFormatting::FormatSong(song, column.format));
        }
    }

    void QueueView::DrawContents(MpdClientWrapper &client)
    {
        ImGuiListClipper clipper;
        clipper.Begin(currentQueue.size());
        if (ImGui::BeginTable("plTable", testColumns.size(), ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY))
        {
            ImGui::TableSetupScrollFreeze(testColumns.size(), 1);
            for (const auto &column: testColumns)
            {
                ImGui::TableSetupColumn(column.displayName.c_str());
            }

            ImGui::TableHeadersRow();

            while (clipper.Step())
            {
                for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
                {
                    auto song = currentQueue[i];
                    auto songId = mpd_song_get_id(song);
                    auto isCurrentSong = songId == currentSongId;
                    if (isCurrentSong)
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImColor(0, 255, 0).Value);
                    }

                    auto &rowCache = cellValueCache[i];
                    CacheRowIfNeeded(song, testColumns, rowCache);

                    ImGui::TableNextRow();

                    for (const auto &value: rowCache)
                    {
                        ImGui::TableNextColumn();
                        //Make a selectable for the whole row as the first column.
                        if (ImGui::TableGetColumnIndex() == 0)
                        {
                            ImGui::PushID(i);
                            if (ImGui::Selectable(value.c_str(), false,
                                                  ImGuiSelectableFlags_SpanAllColumns |
                                                  ImGuiSelectableFlags_AllowDoubleClick)
                                && !ImGui::IsMouseDown(0)
                            )
                            {
                                client.BeginNoIdle();
                                client.PlayId(songId);
                                client.EndNoIdle();
                            }
                            ImGui::PopID();
                        } else
                        {
                            ImGui::Text("%s", value.c_str());
                        }
                    }

                    if (isCurrentSong)
                    {
                        ImGui::PopStyleColor();
                    }
                }
            }
            ImGui::EndTable();
        }
    }

    void QueueView::OnIdleEvent(MpdClientWrapper &client, MpdIdleEventData &data)
    {
        if (data.idleEvent == MPD_IDLE_QUEUE)
        {
            UpdateQueue(&client);
        }
        if (data.idleEvent == MPD_IDLE_PLAYER)
        {
            currentSongId = mpd_status_get_song_id(data.currentStatus);
        }
    }

    void QueueView::InitState(MpdClientWrapper &client)
    {

        client.BeginNoIdle();
        UpdateQueue(&client);
        client.EndNoIdle();
    }

    const std::string QueueView::PanelName()
    {
        return GetFactoryName();
    }
} // ImMPD