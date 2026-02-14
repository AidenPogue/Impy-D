#include "Settings.hpp"

namespace ImpyD::Panels
{
    std::string Settings::PanelName()
    {
        return GetFactoryName();
    }

    void Settings::DrawContents(Context &context)
    {
        //TESTING!
        context.GetConfig().interface.DrawEditor();
    }
}
