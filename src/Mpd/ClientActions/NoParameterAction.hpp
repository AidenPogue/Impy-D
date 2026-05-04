#ifndef IM_MPD_NOPARAMETERACTION_HPP
#define IM_MPD_NOPARAMETERACTION_HPP
#include "IClientAction.hpp"

namespace ImpyD::ClientActions
{
    class NoParameterAction : public IClientAction
    {
        bool (*func)(mpd_connection *);

    public:
        NoParameterAction(bool (*func)(mpd_connection *));

        void Execute(mpd_connection *connection) override;
    };
}

#endif //IM_MPD_NOPARAMETERACTION_HPP
