#ifndef IM_MPD_CONTEXT_HPP
#define IM_MPD_CONTEXT_HPP
#include "Mpd/IdleClientWrapper.hpp"
#include "Mpd/MpdClientWrapper.hpp"

namespace ImpyD
{
    /**
     * Manages the state of an Impy-D session and provides access to the MPD connection and config data.
     */
    class Context
    {
        MpdClientWrapper mainClient;
        Mpd::IdleClientWrapper idleClient;

    public:
        Context(const char *host, unsigned port);

        MpdClientWrapper &GetClient();
        Mpd::IdleClientWrapper &GetIdleClient();
    };
}

#endif