#ifndef IM_MPD_SPECTRUMVISUALIZER_HPP
#define IM_MPD_SPECTRUMVISUALIZER_HPP
#include "PanelBase.hpp"
#include "PeakMeterBar.hpp"
#include "../PanelFactory/RegisterPanel.hpp"
#include <fftw3.h>

namespace ImpyD
{
    class SpectrumVisualizer : public PanelBase, PanelFactory::RegisterPanel<SpectrumVisualizer>
    {
        std::vector<PeakMeterBar> bars;
        PeakMeterBar::Style barStyle;

        int fftSize = 256;
        double *fftIn;
        fftw_complex *fftOut;
        fftw_plan plan;

        double minFreq = 25, maxFreq = 20000;

    public:
        IMPYD_REGISTER_PANEL_FUNCS(SpectrumVisualizer, IMPYD_BUILTIN_ID_PREFIX "spectrum", "Spectrum Visualizer");

        std::string PanelName() override;

    protected:
        void DrawContents(Context &context) override;

    public:
        void InitState(Context &context) override;

        ~SpectrumVisualizer() override;
    };
} // ImpyD

#endif //IM_MPD_SPECTRUMVISUALIZER_HPP