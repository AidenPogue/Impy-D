#pragma once
#include "imgui.h"
#include "PanelFlags.hpp"
#include "../Mpd/MpdClientWrapper.hpp"
#include "../Mpd/MpdIdleEventData.hpp"
#include <string>

namespace ImpyD
{
    class PanelBase
    {
    private:
        bool isOpen = true;

        int callbackId;
        std::string title;
    protected:
        const int panelId = -1;
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
        virtual void DrawContents(MpdClientWrapper &client) {}

        PanelBase(int panelId);

    public:
        virtual PanelFlags GetPanelFlags();

        virtual std::string PanelName() = 0;

        /**
         * Gets if the panel has been closed by the user. Closed panels should be released.
         * @return Whether the panel is closed.
         */
        bool ShouldClose() const;

        /**
         * Sets the title of this panel's window.
         * @param title The new title. If empty, the default name will be used.
         */
        void SetTitle(const std::string &title);

        void Draw(MpdClientWrapper &client);

        virtual ~PanelBase();

        /**
         * Called when any idle event is recieved from the client.
         * @param client The client that produced the idle event.
         * @param event Data for the idle event.
         */
        virtual void OnIdleEvent(MpdClientWrapper &client, mpd_idle event) {}

        /**
         * Called once immediately after creating the panel.
         * @param client The connected client.
         */
        virtual void InitState(MpdClientWrapper &client) {}

        /**
         * Draws context menu items for this panel, if any.
         * @param client The connected client.
         */
        virtual void DrawContextMenu(MpdClientWrapper &client);
    };
} // namespace ImMPD
