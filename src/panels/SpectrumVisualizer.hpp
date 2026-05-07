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

        int fftSize = 2048;
        double *fftIn;
        fftw_complex *fftOut;
        fftw_plan plan;

    public:
        explicit SpectrumVisualizer(int panelId);

        IMPYD_REGISTER_PANEL_FactoryFunc(SpectrumVisualizer);
        IMPYD_REGISTER_PANEL_GetFactoryName("Spectrum Visualizer");

        std::string PanelName() override;

    protected:
        void DrawContents(Context &context) override;

    public:
        void InitState(Context &context) override;

        ~SpectrumVisualizer() override;
    };
} // ImpyD

#endif //IM_MPD_SPECTRUMVISUALIZER_HPP