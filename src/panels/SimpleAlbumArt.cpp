#include "SimpleAlbumArt.hpp"

#include <cmath>
#include <iostream>

#include "imgui.h"
#include "../Utils.hpp"
#include "../Mpd/AlbumArtHelper.hpp"

void ImMPD::SimpleAlbumArt::SetArtwork(const ImpyD::Mpd::AlbumArtHelper::Result &img)
{
    if (currentArtTexture)
    {
        glDeleteTextures(1, &currentArtTexture);
    }

    std::cout << "Uploading" << std::endl;
    ImpyD::Utils::UploadTexture(img.data.get(), img.width, img.height, &currentArtTexture);

    currentArtAspect = (float)img.width / img.height;
}

void ImMPD::SimpleAlbumArt::CheckFutures(MpdClientWrapper &client)
{
    if (ImpyD::Utils::IsReady(artFuture))
    {
        SetArtwork(artFuture.get());
    }
    else if (ImpyD::Utils::IsReady(songFuture))
    {
        auto s = songFuture.get();
        if (!s)
        {
            return;
        }
        auto newUri = s->GetUri();
        if (newUri == currentSongUri)
        {
            return;
        }
        currentSongUri = newUri;
        artFuture = ImpyD::Mpd::AlbumArtHelper::LoadArtworkAsync(client, currentSongUri, false);
    }
}

std::string ImMPD::SimpleAlbumArt::PanelName()
{
    return GetFactoryName();
}

void ImMPD::SimpleAlbumArt::DrawContents(MpdClientWrapper &client)
{
    CheckFutures(client);

    //Can we do this?
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

        //const float loadingBrightness = .5f;
        //auto tint = artFuture.valid() ? ImVec4(loadingBrightness, loadingBrightness, loadingBrightness , 1) : ImVec4(1, 1, 1, 1);
        //ImGui::ImageWithBg((ImTextureID)(intptr_t)currentArtTexture, size, ImVec2(0,0), ImVec2(1,1), ImVec4(), tint);
        ImGui::Image((ImTextureID)(intptr_t)currentArtTexture, size);
    }
}

void ImMPD::SimpleAlbumArt::RequestCurrentArtwork(MpdClientWrapper &client)
{
    songFuture = client.GetCurrentSong();
    artFuture = {};
}

void ImMPD::SimpleAlbumArt::OnIdleEvent(MpdClientWrapper &client, mpd_idle event)
{
    if (event & MPD_IDLE_PLAYER)
    {
        RequestCurrentArtwork(client);
    }
}

void ImMPD::SimpleAlbumArt::InitState(MpdClientWrapper &client)
{
    RequestCurrentArtwork(client);
}

void ImMPD::SimpleAlbumArt::DrawContextMenu(MpdClientWrapper &client)
{
    ImGui::MenuItem("Preserve Aspect Ratio", nullptr, &preserveAspectRatio);
    if (ImGui::BeginMenu("Load Artwork When..."))
    {
        ImGui::MenuItem("Song Changes", nullptr);
        ImGui::MenuItem("Album + Album Artist Changes", nullptr);

        ImGui::EndMenu();
    }

}
