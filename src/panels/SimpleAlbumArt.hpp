#pragma once

#include "PanelBase.hpp"
#include "../Mpd/AlbumArtHelper.hpp"
#include "../PanelFactory/RegisterPanel.hpp"
#include "backends/imgui_impl_opengl3_loader.h"

namespace ImMPD
{
    class SimpleAlbumArt : public ImpyD::PanelBase, ImpyD::PanelFactory::RegisterPanel<SimpleAlbumArt>
    {
        std::future<std::unique_ptr<MpdSongWrapper>> songFuture;
        std::future<ImpyD::Mpd::AlbumArtHelper::Result> artFuture;

        GLuint currentArtTexture = 0;
        float currentArtAspect = 1;
        std::string currentSongUri;
        bool preserveAspectRatio = true;

        void UnloadCurrentArtwork();

        void SetArtwork(const ImpyD::Mpd::AlbumArtHelper::Result &img);

        void CheckFutures(ImpyD::Context &context);

    public:
        IMPYD_REGISTER_PANEL_FUNCS(SimpleAlbumArt, IMPYD_BUILTIN_ID_PREFIX "simple-album-art", "Simple Album Art");

        std::string PanelName() override;

    protected:
        void DrawContents(ImpyD::Context &context) override;

        void RequestCurrentArtwork(ImpyD::Context &context);

    public:
        void OnIdleEvent(ImpyD::Context &context, mpd_idle event) override;

        void InitState(ImpyD::Context &context) override;

        void DrawContextMenu(ImpyD::Context &context) override;
    };
}
