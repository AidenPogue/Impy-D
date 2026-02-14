//
// Created by aiden on 2025-12-10.
//

#include "QueueView.hpp"

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

    void QueueView::UpdateQueue(std::vector<std::unique_ptr<TitleFormatting::ITagged>> songs)
    {
        currentQueue = std::move(songs);
        cellValueCache.resize(currentQueue.size());
        for (auto &row : cellValueCache)
        {
            row.clear();
        }
    }

    void QueueView::CacheRowIfNeeded(std::unique_ptr<TitleFormatting::ITagged> &song, const std::vector<SongTableColumn> &columns,
                                     std::vector<std::string> &rowCache)
    {
        if (rowCache.size() == columns.size())
        {
           return;
        }

        rowCache.clear();

        for (const auto &column : columns)
        {
            rowCache.push_back(TitleFormatting::FormatITagged(*song, column.format));
        }
    }

    void QueueView::DrawContents(Context &context)
    {
        if (Utils::IsReady(queueFuture))
        {
            UpdateQueue(queueFuture.get());
        }

        auto &client = context.GetClient();

        //Buttons
        if (ImGui::Button("Clear"))
        {
            client.ClearQueue();
        }

        ImGui::SameLine();

        if (ImGui::Button("Randomize"))
        {
            client.RandomizeQueue();
        }

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
                    auto &song = currentQueue[i];
                    auto songId = song->GetId();
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
                                client.PlayId(songId);
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

    void QueueView::SetState(Context &context)
    {
        //currentSongId = mpd_status_get_song_id(client.GetStatus().get());
    }

    void QueueView::OnIdleEvent(Context &context, mpd_idle event)
    {
        if (event & MPD_IDLE_QUEUE)
        {
            queueFuture = context.GetClient().GetQueue();
        }
        if (event & MPD_IDLE_PLAYER)
        {
            SetState(context);
        }
    }

    void QueueView::InitState(Context &context)
    {
        queueFuture = context.GetClient().GetQueue();
        SetState(context);
    }

    std::string QueueView::PanelName()
    {
        return GetFactoryName();
    }
} // ImMPD