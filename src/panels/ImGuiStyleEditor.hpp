#ifndef IM_MPD_IMGUISTYLEEDITOR_HPP
#define IM_MPD_IMGUISTYLEEDITOR_HPP
#include "PanelBase.hpp"
#include "../PanelFactory/RegisterPanel.hpp"

namespace ImpyD
{
    class ImGuiStyleEditor : public PanelBase, PanelFactory::RegisterPanel<ImGuiStyleEditor>
    {
    public:
        explicit ImGuiStyleEditor(int panelId);
        IMPYD_REGISTER_PANEL_FactoryFunc(ImGuiStyleEditor);
        IMPYD_REGISTER_PANEL_GetFactoryName("ImGui Style Editor");

        std::string PanelName() override;

    protected:
        void DrawContents(Context &context) override;
    };
}

#endif //IM_MPD_IMGUISTYLEEDITOR_HPP