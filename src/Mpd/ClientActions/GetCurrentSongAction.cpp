#include "GetCurrentSongAction.hpp"

namespace ImpyD::ClientActions
{
    MpdSongWrapper GetCurrentSongAction::GetValue(mpd_connection *connection)
    {
        return mpd_run_current_song(connection);
    }
}
