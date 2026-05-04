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

        std::future<std::unique_ptr<MpdSongWrapper>> songFuture;

        std::vector<std::string> panelsToCreate;

        void DrawLayoutMenu();
        void DrawFileMenu();

        void CreatePanelById(Context &context, const std::string &id);

        void SetWindowTitle(Context &context);

    public:
        MainWindow();

        void Draw(Context &context);

        void SendIdleEventToPanels(Context &context, mpd_idle event);
    };
} // ImpyD

#endif //IM_MPD_MAINWINDOW_HPP