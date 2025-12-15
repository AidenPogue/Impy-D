//
// Created by aiden on 2025-12-09.
//

#include "VolumeControl.hpp"

#include "imgui.h"

namespace ImpyD {
    void VolumeControl::SetState(mpd_status *status)
    {
        if (status == nullptr)
        {
            return;
        }

        currentValue = mpd_status_get_volume(status);
    }

    void VolumeControl::DrawContents(MpdClientWrapper &client)
    {
        //Set these in constructor?
        windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        const auto oldValue = currentValue;

        const auto sliderEdited = ImGui::SliderInt("Vol", &currentValue, 0, 100, "%d%%");
        const auto mouseWheel = ImGui::GetIO().MouseWheel;
        if (!sliderEdited && ImGui::IsItemHovered() && mouseWheel != 0)
        {
            client.BeginNoIdle();
            client.ChangeVolume(2 * (int)mouseWheel);
            client.EndNoIdle();
        }
        else if (currentValue != oldValue)
        {
            client.BeginNoIdle();
            client.SetVolume(currentValue);
            client.EndNoIdle();
        }
    }

    VolumeControl::~VolumeControl()
    {
    }


    void VolumeControl::OnIdleEvent(MpdClientWrapper &client, MpdIdleEventData &data)
    {
        if (data.idleEvent == MPD_IDLE_MIXER)
        {
            SetState(data.currentStatus);
        }
    }

    void VolumeControl::InitState(MpdClientWrapper &client)
    {
        client.BeginNoIdle();
        auto status = client.GetStatus();
        SetState(status);
        if (status != nullptr)
        {
            mpd_status_free(status);
        }
        client.EndNoIdle();
    }

    const std::string VolumeControl::PanelName()
    {
        return GetFactoryName();
    }
} // ImMPD