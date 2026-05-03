#include "SimpleAlbumArt.hpp"

#include <cmath>
#include <iostream>

#include "imgui.h"
#include "../Utils.hpp"
#include "../Mpd/AlbumArtHelper.hpp"

void ImMPD::SimpleAlbumArt::UnloadCurrentArtwork()
{
    if (currentArtTexture)
    {
        glDeleteTextures(1, &currentArtTexture);
    }

    currentArtTexture = 0;
}

void ImMPD::SimpleAlbumArt::SetArtwork(const ImpyD::Mpd::AlbumArtHelper::Result &img)
{
    UnloadCurrentArtwork();

    ImpyD::Utils::UploadTexture(img.data.get(), img.width, img.height, &currentArtTexture);

    currentArtAspect = (float)img.width / img.height;
}

void ImMPD::SimpleAlbumArt::CheckFutures(ImpyD::Context &context)
{
    if (ImpyD::Utils::IsReady(artFuture))
    {
        SetArtwork(artFuture.get());
    }

    if (ImpyD::Utils::IsReady(songFuture))
    {
        auto s = songFuture.get();
        if (!s)
        {
            UnloadCurrentArtwork();
            return;
        }

        auto newUri = s->GetUri();
        if (newUri == currentSongUri)
        {
            return;
        }
        currentSongUri = newUri;
        artFuture = ImpyD::Mpd::AlbumArtHelper::LoadArtworkAsync(context.GetClient(), currentSongUri, false);
    }
}

std::string ImMPD::SimpleAlbumArt::PanelName()
{
    return GetFactoryName();
}

void ImMPD::SimpleAlbumArt::DrawContents(ImpyD::Context &context)
{
    CheckFutures(context);

    if (currentArtTexture)
    {
        auto size = ImGui::GetContentRegionAvail();

        if (preserveAspectRatio)
        {
            auto oldSize = size;
            size.x = oldSize.y * currentArtAspect;

            //Adjust dimensions
            if (size.x > oldSize.x)
            {
                size.x = oldSize.x;
                size.y = size.x / currentArtAspect;
            }

            //Center in window.
            auto available = ImGui::GetContentRegionAvail();

            auto cursor = ImGui::GetCursorPos();
            cursor.x += available.x / 2 - size.x / 2;
            cursor.y += available.y / 2 - size.y / 2;

            cursor.x = std::round(cursor.x);
            cursor.y = std::round(cursor.y);
            size.x = std::round(size.x);
            size.y = std::round(size.y);

            ImGui::SetCursorPos(cursor);
        }

        ImGui::Image((ImTextureID)(intptr_t)currentArtTexture, size);
    }
}

void ImMPD::SimpleAlbumArt::RequestCurrentArtwork(ImpyD::Context &context)
{
    currentSongUri = "";
    songFuture = context.GetClient().GetCurrentSong();
    artFuture = {};
}

void ImMPD::SimpleAlbumArt::OnIdleEvent(ImpyD::Context &context, mpd_idle event)
{
    if (event & MPD_IDLE_PLAYER)
    {
        RequestCurrentArtwork(context);
    }
}

void ImMPD::SimpleAlbumArt::InitState(ImpyD::Context &context)
{
    RequestCurrentArtwork(context);
}

void ImMPD::SimpleAlbumArt::DrawContextMenu(ImpyD::Context &context)
{
    ImGui::MenuItem("Preserve Aspect Ratio", nullptr, &preserveAspectRatio);
    if (ImGui::BeginMenu("Load Artwork When..."))
    {
        ImGui::MenuItem("Song Changes", nullptr);
        ImGui::MenuItem("Album + Album Artist Changes", nullptr);

        ImGui::EndMenu();
    }

}
