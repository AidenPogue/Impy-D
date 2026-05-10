#include "PanelRegistry.hpp"
#include "RegisteredPanelInfo.hpp"

#ifndef IM_MPD_REGISTERPANEL_HPP
#define IM_MPD_REGISTERPANEL_HPP

#define IMPYD_REGISTER_PANEL_FUNCS(typeName, panelId, displayName) static std::unique_ptr<PanelBase> FactoryFunc(int id) {return std::make_unique<typeName>(id);} static const std::string GetFactoryName() {return displayName;} static const std::string GetFactoryId() {return panelId;}

// Don't use, unless you're me! It would make me very sad
#define IMPYD_BUILTIN_ID_PREFIX "impy-d.builtin."

namespace ImpyD::PanelFactory
{
    template<class T>
    class RegisterPanel
    {
    public:
        //Need to reference panelRegistered to make sure the compiler doesn't optimize it out.
        RegisterPanel() { panelRegistered; };

        ~RegisterPanel()
        {
        };

    private:
        static bool panelRegistered;
    };

    template <class T> bool RegisterPanel<T>::panelRegistered = (Registry::RegisterPanel(RegisteredPanelInfo(T::GetFactoryId(), T::GetFactoryName(), T::FactoryFunc)), true);
}



#endif //IM_MPD_REGISTERPANEL_HPP