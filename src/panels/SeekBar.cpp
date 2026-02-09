#include "SeekBar.hpp"
#include "imgui.h"
#include "../Utils.hpp"
#include "GLFW/glfw3.h"

using namespace std::chrono_literals;

void ImpyD::SeekBar::DrawContents(MpdClientWrapper &client)
{
    CheckFutures();

    bool disabled = currentState == MPD_STATE_STOP || currentState == MPD_STATE_UNKNOWN;

    ImGui::BeginDisabled(disabled);

    bool seeking = currentSeek >= 0;

    currentSeek = seeking ? currentSeek : currentElapsedSeconds;

    if (!seeking && currentState == MPD_STATE_PLAY)
    {
        currentSeek += glfwGetTime() - elapsedSecondsSetAtTime;
    }

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##", &currentSeek, 0, currentDuration, Utils::SecondsToDurationString(currentSeek).c_str());

    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        printf("Seeking to %f\n", currentSeek);

                client.SeekToSeconds(currentSeek, false);
            }

    if (!ImGui::IsItemActive())
    {
        currentSeek = -1;
    }

    ImGui::EndDisabled();

}

void ImpyD::SeekBar::OnIdleEvent(MpdClientWrapper &client, mpd_idle event)
{
    if (event & MPD_IDLE_PLAYER | MPD_IDLE_QUEUE)
    {
        GetFutures(client);
    }
}

void ImpyD::SeekBar::InitState(MpdClientWrapper &client)
{
    windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    GetFutures(client);
}

std::string ImpyD::SeekBar::PanelName()
{
    return GetFactoryName();
}

void ImpyD::SeekBar::GetFutures(MpdClientWrapper &client)
{
    songFuture = client.GetCurrentSong();
    statusFuture = client.GetStatus();
}

void ImpyD::SeekBar::CheckFutures()
{
    if (Utils::IsReady(songFuture) && Utils::IsReady(statusFuture))
    {
        SetState(songFuture.get(), statusFuture.get());
    }
}

void ImpyD::SeekBar::SetState(const std::unique_ptr<MpdSongWrapper> &song, const MpdClientWrapper::MpdStatusPtr &status)
{
    if (status == nullptr)
    {
        return;
    }

    currentState = mpd_status_get_state(status.get());
    if (currentState == MPD_STATE_PLAY || currentState == MPD_STATE_PAUSE)
    {
        currentElapsedSeconds = mpd_status_get_elapsed_ms(status.get()) / 1000.0f;
        currentDuration = song->GetDurationMs() / 1000.0f;
        elapsedSecondsSetAtTime = ImGui::GetTime();
    }
    else
    {
        currentElapsedSeconds = currentDuration = 0;
        elapsedSecondsSetAtTime = 0;
    }
}

ImpyD::SeekBar::~SeekBar()
= default;
