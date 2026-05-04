#include "PeakMeterBar.hpp"


namespace ImpyD
{

    const PeakMeterBar::Style PeakMeterBar::defaultStyle;

    float LerpMainAxisValue(float min, float max, float value, bool reverse)
    {
        return std::lerp(min, max, reverse ? 1 - value : value);
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

        auto fillMax = max, fillMin = min;

        if (!useStyle->reverse)
        {
            if (vertical)
            {
                fillMax.y = std::lerp(min.y, max.y, value);
            }
            else
            {
                fillMax.x = std::lerp(min.x, max.x, value);
            }
        }
        else
        {
            if (vertical)
            {
                fillMin.y = std::lerp(min.y, max.y, 1 - value);
            }
            else
            {
                fillMin.x = std::lerp(min.x, max.x, 1 - value);
            }

        }

        drawList->AddRectFilled(fillMin, fillMax, useStyle->foregroundColor, round);

        auto peakMin = min;
        peakMin.x = std::lerp(min.x, max.x, peak);
        auto peakMax = peakMin;
        peakMax.x += useStyle->peakWidth;
        peakMax.y = max.y;
        drawList->AddRectFilled(peakMin, peakMax, useStyle->peakColor);
    }
} // ImpyD