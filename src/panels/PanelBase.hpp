#pragma once
#include <utility>

#include "imgui.h"
#include "../MpdClientWrapper.hpp"

namespace ImpyD
{
    class PanelBase
    {
    private:
        bool isOpen = true;
        const int panelId = -1;
        int callbackId;
        std::string title;
    protected:
        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;

        virtual void DrawContents(MpdClientWrapper &client) {}

        PanelBase(int panelId);

    public:
        virtual const std::string PanelName() = 0;

        /**
         * Gets if the panel has been closed by the user. Closed panels should be released.
         * @return Whether the panel is closed.
         */
        bool ShouldClose() const { return !isOpen; }

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
         * @param data Data for the idle event.
         */
        virtual void OnIdleEvent(MpdClientWrapper &client, MpdIdleEventData &data) {}

        /**
         * Called once immediately after creating the panel.
         * @param client The connected client.
         */
        virtual void InitState(MpdClientWrapper &client) {}

        void RegisterCallbacks(MpdClientWrapper &client);
    };
} // namespace ImMPD
