#ifndef IM_MPD_CONTEXT_HPP
#define IM_MPD_CONTEXT_HPP
#include "Config.hpp"
#include "Mpd/IdleClientWrapper.hpp"
#include "Mpd/MpdClientWrapper.hpp"
#include "ConfigLoader.hpp"

namespace ImpyD
{
    /**
     * Manages the state of an Impy-D session and provides access to the MPD connection and config data.
     */
    class Context
    {
        MpdClientWrapper mainClient;
        Mpd::IdleClientWrapper idleClient;

        Config config;

    public:
        Context(const char *host, unsigned port);

        MpdClientWrapper &GetClient();
        Mpd::IdleClientWrapper &GetIdleClient();

        Config &GetConfig();
    };
}

#endif