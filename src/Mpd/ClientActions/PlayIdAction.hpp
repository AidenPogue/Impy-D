#ifndef IM_MPD_PLAYIDACTION_HPP
#define IM_MPD_PLAYIDACTION_HPP
#include "IClientAction.hpp"

namespace ImpyD::ClientActions
{
    class PlayIdAction : public IClientAction
    {
        const unsigned id;
    public:
        explicit PlayIdAction(unsigned id);

        void Execute(mpd_connection *connection) override;
    };
}

#endif //IM_MPD_PLAYIDACTION_HPP
