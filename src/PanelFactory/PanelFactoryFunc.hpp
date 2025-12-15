#ifndef IM_MPD_PANELFACTORYFUNC_HPP
#define IM_MPD_PANELFACTORYFUNC_HPP
#include <memory>

#include "../panels/PanelBase.hpp"

namespace ImpyD::PanelFactory
{
    using PanelFactoryFunc = std::unique_ptr<PanelBase>(*)(int id);
}
#endif //IM_MPD_PANELFACTORYFUNC_HPP