#pragma once

#include "PanelBase.hpp"
#include "../Mpd/AlbumArtHelper.hpp"
#include "../PanelFactory/RegisterPanel.hpp"
#include "backends/imgui_impl_opengl3_loader.h"

namespace ImMPD
{
    class SimpleAlbumArt : public ImpyD::PanelBase, ImpyD::PanelFactory::RegisterPanel<SimpleAlbumArt>
    {
    private:
        std::future<std::unique_ptr<MpdSongWrapper>> songFuture;
        std::future<ImpyD::Mpd::AlbumArtHelper::Result> artFuture;

        GLuint currentArtTexture = 0;
        float currentArtAspect = 1;
        std::string currentSongUri;
        bool preserveAspectRatio = true;

        void SetArtwork(const ImpyD::Mpd::AlbumArtHelper::Result &img);

        void CheckFutures(MpdClientWrapper &client);

    public:
        explicit SimpleAlbumArt(int panelId)
            : PanelBase(panelId)
        {
        }

        IMPYD_REGISTER_PANEL_FactoryFunc(SimpleAlbumArt);
        IMPYD_REGISTER_PANEL_GetFactoryName("Simple Album Art");

        std::string PanelName() override;

    protected:
        void DrawContents(MpdClientWrapper &client) override;

        void RequestCurrentArtwork(MpdClientWrapper &client);

    public:
        void OnIdleEvent(MpdClientWrapper &client, mpd_idle event) override;

        void InitState(MpdClientWrapper &client) override;

        void DrawContextMenu(MpdClientWrapper &client) override;
    };
}
