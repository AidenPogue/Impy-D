//
// Created by aiden on 2025-12-09.
//

#include "VolumeControl.hpp"

#include "imgui.h"

namespace ImpyD {
    void VolumeControl::SetState(const MpdClientWrapper::MpdStatusPtr &status)
    {
        if (status == nullptr)
        {
            return;
        }

        currentValue = mpd_status_get_volume(status.get());
    }

    void VolumeControl::DrawContents(MpdClientWrapper &client)
    {
        //Set these in constructor?
        windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;

        const auto oldValue = currentValue;

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        const auto sliderEdited = ImGui::SliderInt("##", &currentValue, 0, 100, "%d%%");
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


    void VolumeControl::OnIdleEvent(MpdClientWrapper &client, mpd_idle event)
    {
        if (event & MPD_IDLE_MIXER)
        {
            SetState(client.GetStatus());
        }
    }

    void VolumeControl::InitState(MpdClientWrapper &client)
    {
        client.BeginNoIdle();
        auto& status = client.GetStatus();
        SetState(status);
        client.EndNoIdle();
    }

    std::string VolumeControl::PanelName()
    {
        return GetFactoryName();
    }
} // ImMPD