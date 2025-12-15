#include "PanelRegistry.hpp"
#include "RegisteredPanelInfo.hpp"

#ifndef IM_MPD_REGISTERPANEL_HPP
#define IM_MPD_REGISTERPANEL_HPP

#define IMPYD_REGISTER_PANEL_FactoryFunc(typeName) static std::unique_ptr<PanelBase> FactoryFunc(int id) {return std::make_unique<typeName>(id);}
#define IMPYD_REGISTER_PANEL_GetFactoryName(name) static const std::string GetFactoryName() {return name;}

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

    template <class T> bool RegisterPanel<T>::panelRegistered = (Registry::RegisterPanel(RegisteredPanelInfo(typeid(T).name(), T::GetFactoryName(), T::FactoryFunc)), true);
}



#endif //IM_MPD_REGISTERPANEL_HPP