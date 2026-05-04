#include "VolumeMeter.hpp"
#include <poll.h>

std::string ImpyD::VolumeMeter::PanelName()
{
    return GetFactoryName();
}

void ImpyD::VolumeMeter::DrawContents(Context &context)
{
    auto sampleWindow = 200;

    int lTarget = 0, rTarget = 0;
    //left = right = 0;
    for (int i = 0; i < sampleWindow * 2; i += 2)
    {
        auto idx = context.GetFifoReader().GetFrontIndex() - i;

        lTarget = std::max(std::abs(context.GetFifoReader().buffer[idx % context.GetFifoReader().buffer.size()]), lTarget);
        rTarget = std::max(std::abs(context.GetFifoReader().buffer[(idx - 1) % context.GetFifoReader().buffer.size()]), rTarget);
    }

    left = std::lerp(left, lTarget, .25f);
    right = std::lerp(right, rTarget, .25f);

    auto lDBFS = 20 * std::log10(left / 32768.0f), rDBFS = 20 * std::log10(right / 32768.0f);

    const float minDBFS = -60;
    auto percentL = (lDBFS - minDBFS) / (-minDBFS), percentR = (rDBFS - minDBFS) / (-minDBFS);

    auto tl1 = ImGui::GetCursorScreenPos();
    auto br1 = ImVec2(tl1.x + ImGui::GetContentRegionAvail().x, tl1.y + ImGui::GetContentRegionAvail().y / 2);
    leftBar.Draw(context, tl1, br1, percentL, false);

    auto tl2 = ImVec2(tl1.x, br1.y + 2);
    auto br2 = ImVec2(tl1.x + ImGui::GetContentRegionAvail().x, tl2.y + ImGui::GetContentRegionAvail().y / 2);
    rightBar.Draw(context, tl2, br2, percentR, false);
}

void ImpyD::VolumeMeter::InitState(Context &context)
{

}
