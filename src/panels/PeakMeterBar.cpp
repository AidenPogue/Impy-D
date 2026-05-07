#include "PeakMeterBar.hpp"


namespace ImpyD
{

    const PeakMeterBar::Style PeakMeterBar::defaultStyle;

    struct MinMax
    {
        ImVec2 min, max;
    };

    MinMax LerpRect(ImVec2 containerMin, ImVec2 containerMax, float t1, float t2, bool vertical)
    {
        if (vertical)
        {
            auto min = ImVec2(containerMin.x, std::lerp(containerMin.y, containerMax.y, t1));
            auto max = ImVec2(containerMax.x, std::lerp(containerMin.y, containerMax.y, t2));
            return {min, max};
        }
        else
        {
            auto min = ImVec2(std::lerp(containerMin.x, containerMax.x, t1), containerMin.y);
            auto max = ImVec2(std::lerp(containerMin.x, containerMax.x, t2), containerMax.y);
            return {min, max};
        }
    }

    void PeakMeterBar::Draw(Context &context, ImVec2 min, ImVec2 max, float value, bool vertical)
    {
        auto useStyle = style ? style : &defaultStyle;

        peakTime += context.GetDeltaTime();

        if (peakTime > useStyle->peakFallDelay)
        {
            isFalling = true;
        }

        if (isFalling)
        {
            peak -= useStyle->peakFallSpeed * context.GetDeltaTime();
        }

        value = std::lerp(lastValue, value, useStyle->valueSmoothT);
        value = std::clamp(value, 0.0f, 1.0f);
        lastValue = value;

        if (value > peak)
        {
            isFalling = false;
            peak = value;
            peakTime = 0;
        }

        auto drawList = ImGui::GetWindowDrawList();
        auto round = useStyle->rounding;
        drawList->AddRectFilled(min, max, useStyle->backgroundColor, round);

        MinMax fillSize, peakSize;

        if (useStyle->reverse)
        {
            fillSize = LerpRect(min, max, 1 - value, 1, vertical);
        }
        else
        {
            fillSize = LerpRect(min, max, 0, value, vertical);
        }


        drawList->AddRectFilled(fillSize.min, fillSize.max, useStyle->foregroundColor, round);

        auto peakMin = min;
        peakMin.x = std::lerp(min.x, max.x, peak);
        auto peakMax = peakMin;
        peakMax.x += useStyle->peakWidth;
        peakMax.y = max.y;
        //drawList->AddRectFilled(peakMin, peakMax, useStyle->peakColor);
    }
} // ImpyD