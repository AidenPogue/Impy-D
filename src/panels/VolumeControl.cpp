//
// Created by aiden on 2025-12-09.
//

#include "VolumeControl.hpp"

#include "imgui.h"
#include "../Utils.hpp"

namespace ImpyD {
    void VolumeControl::SetState(const MpdClientWrapper::MpdStatusPtr &status)
    {
        currentValue = status ? mpd_status_get_volume(status.get()) : -1;
    }

    VolumeControl::VolumeControl(int panelId): PanelBase(panelId)
    {
        windowFlags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
    }

    void VolumeControl::DrawContents(Context &context)
    {
        if (Utils::IsReady(statusFuture))
        {
            SetState(statusFuture.get());
        }

        const auto oldValue = currentValue;

        ImGui::BeginDisabled(currentValue == -1);

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        const auto sliderEdited = ImGui::SliderInt("##", &currentValue, 0, 100, "%d%%");
        const auto mouseWheel = ImGui::GetIO().MouseWheel;
        if (!sliderEdited && ImGui::IsItemHovered() && mouseWheel != 0)
        {
            //TODO: Configurable change amount.
            context.GetClient().ChangeVolume(2 * (int) mouseWheel);
        }
        else if (currentValue != oldValue)
        {
            context.GetClient().SetVolume(currentValue);
        }

        ImGui::EndDisabled();
    }

    VolumeControl::~VolumeControl()
    {
    }


    void VolumeControl::OnIdleEvent(Context &context, mpd_idle event)
    {
        if (event & (MPD_IDLE_MIXER | MPD_IDLE_PLAYER))
        {
            InitState(context);
        }
    }

    void VolumeControl::InitState(Context &context)
    {
        statusFuture = context.GetClient().GetStatus();
    }

    std::string VolumeControl::PanelName()
    {
        return GetFactoryName();
    }
} // ImMPD