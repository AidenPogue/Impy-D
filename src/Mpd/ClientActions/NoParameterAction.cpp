#include "NoParameterAction.hpp"

#include <assert.h>

namespace ImpyD::ClientActions
{
    NoParameterAction::NoParameterAction(bool (*func)(mpd_connection *)) : func(func)
    {
    }

    void NoParameterAction::Execute(mpd_connection *connection)
    {
        assert(func);
        func(connection);
    }
} // ImpyD