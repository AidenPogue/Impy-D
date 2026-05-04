#ifndef IM_MPD_PEAKMETERBAR_HPP
#define IM_MPD_PEAKMETERBAR_HPP
#include "imgui.h"
#include "../Context.hpp"

namespace ImpyD
{

    // Draws a filled bar based on a 0-1 percentage with a falling peak.
    class PeakMeterBar
    {
    public:
        struct Style
        {
            ImU32 backgroundColor = IM_COL32(50, 50, 50, 255), foregroundColor = IM_COL32(0, 255, 0, 255), peakColor = IM_COL32(255, 0, 0, 255);
            float rounding = 0;

            float peakFallSpeed = .125f, peakFallDelay = .35f;
            float peakWidth = 1;
            bool showPeak = true;

            // (0, 1] value used as the t value for a lerp between the previous and current value to smooth it out.
            float valueSmoothT = .3f;

            bool reverse = false;
        };

        static const Style defaultStyle;

    private:
        float peak = -1;
        float lastValue = 0;
        bool isFalling = true;
        double peakTime = 0;

    public:
        void Draw(Context &context, ImVec2 min, ImVec2 max, float value, bool vertical);
        const Style *style = nullptr;
    };
} // ImpyD

#endif //IM_MPD_PEAKMETERBAR_HPP