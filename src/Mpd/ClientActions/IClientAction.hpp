#ifndef IM_MPD_ICLIENTACTION_HPP
#define IM_MPD_ICLIENTACTION_HPP

#include <mpd/connection.h>

namespace ImpyD::ClientActions
{
    class IClientAction
    {
    public:
        IClientAction() = default;

        virtual ~IClientAction() = default;

        virtual void Execute(mpd_connection *connection) = 0;
    };
} // ImpyD

#endif //IM_MPD_ICLIENTACTION_HPP