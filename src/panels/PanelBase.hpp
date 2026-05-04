#pragma once
#include "imgui.h"
#include "PanelFlags.hpp"
#include "../Mpd/MpdClientWrapper.hpp"
#include "../Context.hpp"
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
        virtual void DrawContents(Context &context) {}

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

        void Draw(Context &context);

        virtual ~PanelBase();

        /**
         * Called when any idle event is recieved from the client.
         * @param context The client that produced the idle event.
         * @param event Data for the idle event.
         */
        virtual void OnIdleEvent(Context &context, mpd_idle event) {}

        /**
         * Called once immediately after creating the panel.
         * @param context The connected client.
         */
        virtual void InitState(Context &context) {}

        /**
         * Draws context menu items for this panel, if any.
         * @param context The connected client.
         */
        virtual void DrawContextMenu(Context &context);
    };
} // namespace ImMPD
