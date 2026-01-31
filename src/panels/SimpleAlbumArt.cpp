#include "SimpleAlbumArt.hpp"

#include "imgui.h"
#include "../Utils.hpp"
#include "../Mpd/AlbumArtHelper.hpp"

void ImMPD::SimpleAlbumArt::SetArtwork(MpdClientWrapper &client, const std::string &uri)
{
    if (currentSongUri == uri)
    {
        return;
    }

    if (currentArtTexture)
    {
        glDeleteTextures(1, &currentArtTexture);
    }

    ImVec2 dimensions;
    currentArtTexture = ImpyD::Mpd::AlbumArtHelper::LoadArtworkToGPU(client, uri, dimensions, false);
    currentArtAspect = dimensions.x / dimensions.y;
    currentSongUri = uri;
}

std::string ImMPD::SimpleAlbumArt::PanelName()
{
    return GetFactoryName();
}

void ImMPD::SimpleAlbumArt::DrawContents(MpdClientWrapper &client)
{
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
            ImGui::SetCursorPos(cursor);
        }

        //Double cast baybee
        ImGui::Image((ImTextureID)(intptr_t)currentArtTexture, size);
    }
}

void ImMPD::SimpleAlbumArt::SetCurrentArtwork(MpdClientWrapper &client)
{
    auto &curSong = client.GetCurrentSong();
    if (curSong)
    {
        SetArtwork(client, mpd_song_get_uri(curSong.get()));
    }
    else
    {
        currentArtTexture = 0;
    }
}

void ImMPD::SimpleAlbumArt::OnIdleEvent(MpdClientWrapper &client, mpd_idle event)
{
    if (event & MPD_IDLE_PLAYER)
    {
        SetCurrentArtwork(client);
    }
}

void ImMPD::SimpleAlbumArt::InitState(MpdClientWrapper &client)
{
    client.BeginNoIdle();
    SetCurrentArtwork(client);
    client.EndNoIdle();
}

void ImMPD::SimpleAlbumArt::DrawContextMenu(MpdClientWrapper &client)
{
    ImGui::MenuItem("Preserve Aspect Ratio", nullptr, &preserveAspectRatio);
}
