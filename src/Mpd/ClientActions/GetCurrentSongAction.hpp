#ifndef IM_MPD_GETCURRENTSONG_HPP
#define IM_MPD_GETCURRENTSONG_HPP
#include "PromiseAction.hpp"
#include "../MpdSongWrapper.hpp"

namespace ImpyD::ClientActions
{
    class GetCurrentSongAction : public PromiseAction<MpdSongWrapper>
    {
    protected:
        MpdSongWrapper GetValue(mpd_connection *connection) override;
    };
}

#endif //IM_MPD_GETCURRENTSONG_HPP
