#include "SeekBar.hpp"

#include <iostream>

#include "imgui.h"
#include "../Utils.hpp"
#include "GLFW/glfw3.h"

using namespace std::chrono_literals;

void ImpyD::SeekBar::DrawContents(Context &context)
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

    //Elapsed
    ImGui::Text(showRemainingTime ? "-%s" : "%s", Utils::SecondsToDurationString(showRemainingTime ? currentDuration - currentSeek : currentSeek).c_str());

    auto durationSize = ImGui::CalcTextSize(currentDurationString.c_str());

    ImGui::SameLine();

    //Seekbar
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - durationSize.x - ImGui::GetStyle().WindowPadding.x);
    ImGui::SliderFloat("##", &currentSeek, 0, currentDuration, "");

    if (ImGui::IsItemDeactivatedAfterEdit())
    {
        std::cout << "Seeking to " << currentSeek << std::endl;
        context.GetClient().SeekToSeconds(currentSeek, false);
    }

    if (!ImGui::IsItemActive())
    {
        currentSeek = -1;
    }

    ImGui::SameLine();

    //Duration
    ImGui::Text("%s", currentDurationString.c_str());

    ImGui::EndDisabled();

}

void ImpyD::SeekBar::OnIdleEvent(Context &context, mpd_idle event)
{
    if (event & MPD_IDLE_PLAYER | MPD_IDLE_QUEUE)
    {
        GetFutures(context);
    }
}

void ImpyD::SeekBar::InitState(Context &context)
{
    windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

    GetFutures(context);
}

std::string ImpyD::SeekBar::PanelName()
{
    return GetFactoryName();
}

void ImpyD::SeekBar::DrawContextMenu(Context &context)
{
    ImGui::MenuItem("Display Remaining Time", nullptr, &showRemainingTime);
}

void ImpyD::SeekBar::GetFutures(Context &context)
{
    auto &client = context.GetClient();
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
        currentDurationString = Utils::SecondsToDurationString(currentDuration);
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
