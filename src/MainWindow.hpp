#ifndef IM_MPD_MAINWINDOW_HPP
#define IM_MPD_MAINWINDOW_HPP
#include <list>
#include <memory>

#include "Mpd/MpdClientWrapper.hpp"
#include "Mpd/MpdIdleEventData.hpp"
#include "panels/PanelBase.hpp"
#include "Context.hpp"

namespace ImpyD
{
    class MainWindow
    {
    private:
        std::list<std::unique_ptr<PanelBase>> panels;
        int nextPanelId = 0;

        std::vector<std::string> panelsToCreate;

        void DrawLayoutMenu();
        void DrawFileMenu();

        void CreatePanelById(Context &context, const std::string &id);

    public:
        MainWindow();

        void Draw(Context &context);

        void SendIdleEventToPanels(Context &context, mpd_idle event) const;
    };
} // ImpyD

#endif //IM_MPD_MAINWINDOW_HPP