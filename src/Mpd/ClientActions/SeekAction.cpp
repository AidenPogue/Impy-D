#include "SeekAction.hpp"

#include "GetCurrentSongAction.hpp"

namespace ImpyD::ClientActions
{
    SeekAction::SeekAction(float seconds, bool relative): seconds(seconds), relative(relative)
    {
    }

    void SeekAction::Execute(mpd_connection *connection)
    {
        mpd_run_seek_current(connection, seconds, relative);
    }
}
