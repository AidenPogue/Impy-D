#ifndef IM_MPD_CONTEXT_HPP
#define IM_MPD_CONTEXT_HPP
#include "Config.hpp"
#include "Mpd/IdleClientWrapper.hpp"
#include "Mpd/MpdClientWrapper.hpp"
#include "ConfigLoader.hpp"
#include "FifoReader.hpp"

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
        FifoReader fifoReader;

        double lastTickTime;
        double deltaTime;

    public:
        Context(const char *host, unsigned port);

        MpdClientWrapper &GetClient();
        Mpd::IdleClientWrapper &GetIdleClient();
        FifoReader &GetFifoReader();

        Config &GetConfig();

        double GetDeltaTime();

        void Tick();
    };
}

#endif