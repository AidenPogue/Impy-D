#include "ChangeVolumeAction.hpp"

#include <mpd/mixer.h>

namespace ImpyD::ClientActions
{
    ChangeVolumeAction::ChangeVolumeAction(int value, Mode mode) : value(value), mode(mode)
    {
    }

    void ChangeVolumeAction::Execute(mpd_connection *connection)
    {
        if (mode == Mode::Set)
        {
            mpd_run_set_volume(connection, value);
        }
        else
        {
            mpd_run_change_volume(connection, value);
        }
    }
}
