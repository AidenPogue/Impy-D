#include "SeekBar.hpp"
#include "imgui.h"
#include "GLFW/glfw3.h"


void ImpyD::SeekBar::DrawContents(MpdClientWrapper &client)
{
    bool disabled = currentState == MPD_STATE_STOP || currentState == MPD_STATE_UNKNOWN;

    ImGui::BeginDisabled(disabled);

    bool seeking = currentSeek >= 0;

    currentSeek = seeking ? currentSeek : currentElapsedSeconds;

    if (!seeking && currentState == MPD_STATE_PLAY)
    {
        currentSeek += glfwGetTime() - elapsedSecondsSetAtTime;
    }

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##", &currentSeek, 0, currentDuration);

    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        printf("Seeking to %f\n", currentSeek);

        client.BeginNoIdle();
        client.SeekToSeconds(currentSeek, false);
        client.EndNoIdle();
    }

    if (!ImGui::IsItemActive())
    {
        currentSeek = -1;
    }

    ImGui::EndDisabled();

}

void ImpyD::SeekBar::OnIdleEvent(MpdClientWrapper &client, MpdIdleEventData &data)
{
    puts("Seekbar idle");
    if ((data.idleEvent & MPD_IDLE_PLAYER) != 0)
    {
        SetState(data.currentSong, data.currentStatus);
    }
}

void ImpyD::SeekBar::InitState(MpdClientWrapper &client)
{
    windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    client.BeginNoIdle();
    auto song = client.GetCurrentSong();
    auto status = client.GetStatus();
    SetState(song, status);

    if (song != nullptr)
    {
        mpd_song_free(song);
    }
    if (status != nullptr)
    {
        mpd_status_free(status);
    }
    client.EndNoIdle();
}

const std::string ImpyD::SeekBar::PanelName()
{
    return GetFactoryName();
}

void ImpyD::SeekBar::SetState(mpd_song *song, mpd_status *status)
{
    if (song == nullptr || status == nullptr)
    {
        return;
    }

    currentState = mpd_status_get_state(status);
    if (currentState == MPD_STATE_PLAY || currentState == MPD_STATE_PAUSE)
    {
        currentElapsedSeconds = mpd_status_get_elapsed_ms(status) / 1000.0f;
        currentDuration = mpd_song_get_duration_ms(song) / 1000.0f;
        elapsedSecondsSetAtTime = glfwGetTime();
    }
    else
    {
        currentElapsedSeconds = currentDuration = 0;
        elapsedSecondsSetAtTime = 0;
    }
}

ImpyD::SeekBar::~SeekBar()
= default;
