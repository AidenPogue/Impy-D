#pragma once

#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"
#include "backends/imgui_impl_opengl3_loader.h"

namespace ImMPD
{
    class SimpleAlbumArt : public ImpyD::PanelBase, ImpyD::PanelFactory::RegisterPanel<SimpleAlbumArt>
    {
    private:
        GLuint currentArtTexture = 0;
        float currentArtAspect = 1;
        bool preserveAspectRatio = true;
        uint8_t *receiveBuffer = NULL;

        void SetArtwork(MpdClientWrapper &client, const std::string &uri);

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

        void SetCurrentArtwork(MpdClientWrapper &client);

    public:
        void OnIdleEvent(MpdClientWrapper &client, mpd_idle event) override;

        void InitState(MpdClientWrapper &client) override;
    };
}
