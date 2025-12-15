#ifndef IM_MPD_MAINWINDOW_HPP
#define IM_MPD_MAINWINDOW_HPP
#include <memory>

#include "MpdClientWrapper.hpp"
#include "panels/PanelBase.hpp"

namespace ImpyD
{
    class MainWindow
    {
    private:
        std::list<std::unique_ptr<PanelBase>> panels;
        int nextPanelId = 0;

        std::vector<std::string> panelsToCreate;

        void DrawLayoutMenu();

        void CreatePanelById(MpdClientWrapper &client, const std::string &id);

    public:
        MainWindow();

        void Draw(MpdClientWrapper &client);
    };
} // ImpyD

#endif //IM_MPD_MAINWINDOW_HPP