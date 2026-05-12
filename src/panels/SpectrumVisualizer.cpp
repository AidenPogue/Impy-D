#include "SpectrumVisualizer.hpp"

#include <iostream>

#include "../Utils.hpp"

namespace ImpyD
{
    std::string SpectrumVisualizer::PanelName()
    {
        return GetFactoryName();
    }

    void SpectrumVisualizer::DrawContents(Context &context)
    {
        auto front = context.GetFifoReader().GetFrontIndex();
        for (int i = 0; i < fftSize * 2; i += 2)
        {
            auto &fifo = context.GetFifoReader();
            auto l = fifo.buffer[(front - i) % fifo.buffer.size()] / 32768.0, r = fifo.buffer[(front - i - 1) % fifo.buffer.size()] / 32768.0;
            fftIn[i] = (l + r) / 2;
        }

        fftw_execute(plan);

        auto start = ImGui::GetCursorScreenPos();
        auto avail = ImGui::GetContentRegionAvail();
        auto barWidth = avail.x / bars.size();

        auto maxSample = 0.0;
        for (int i = 0; i < bars.size(); ++i)
        {
            auto min = ImVec2(start.x + i * barWidth, start.y);
            auto max = ImVec2(start.x + (i + 1) * barWidth, start.y + avail.y);

            auto percent = (double)i / bars.size();

            auto fftSeconds = (double)fftSize / context.GetConfig().visualizer.fifoSampleRate;
            auto freq = minFreq * fftSeconds * std::pow((maxFreq * fftSeconds) / (minFreq * fftSeconds), percent);

            auto floor = int(std::floor(freq)), ceil = int(std::ceil(freq));
            auto t = freq - floor;

            auto r1 = fftOut[floor][0], imag1 = fftOut[floor][1], r2 = fftOut[ceil][0], imag2 = fftOut[ceil][1];
            auto r = std::lerp(r1, r2, t), imag = std::lerp(imag1, imag2, t);
            //auto r = fftOut[floor][0], imag = fftOut[floor][1];
            bars[i].Draw(context, min, max, std::sqrt(r * r + imag * imag) / bars.size(), true);
        }

    }

    void SpectrumVisualizer::InitState(Context &context)
    {
        bars.resize(fftSize / 2 + 1);
        barStyle.reverse = true;
        barStyle.backgroundColor = IM_COL32(0,0,0,0);
        for (auto &bar: bars)
        {
            bar.style = &barStyle;
        }

        fftIn = fftw_alloc_real(sizeof(double) * fftSize);
        fftOut = fftw_alloc_complex(sizeof(fftw_complex) * bars.size());
        plan = fftw_plan_dft_r2c_1d(fftSize, fftIn, fftOut, FFTW_MEASURE);
    }

    SpectrumVisualizer::~SpectrumVisualizer()
    {
        fftw_free(fftIn);
        fftw_free(fftOut);
        fftw_destroy_plan(plan);
    }
} // ImpyD