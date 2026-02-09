#include "PlayIdAction.hpp"

#include <mpd/player.h>

namespace ImpyD::ClientActions
{
    PlayIdAction::PlayIdAction(unsigned id): id(id)
    {
    }

    void PlayIdAction::Execute(mpd_connection *connection)
    {
        mpd_run_play_id(connection, id);
    }
}
