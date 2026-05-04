#ifndef IM_MPD_SEEKACTION_HPP
#define IM_MPD_SEEKACTION_HPP
#include "IClientAction.hpp"

namespace ImpyD::ClientActions
{
    class SeekAction : public IClientAction
    {
        const float seconds;
        const bool relative;

    public:
        SeekAction(float seconds, bool relative);

        void Execute(mpd_connection *connection) override;
    };
}

#endif //IM_MPD_SEEKACTION_HPP
