#ifndef IM_MPD_IDLECLIENTWRAPPER_HPP
#define IM_MPD_IDLECLIENTWRAPPER_HPP
#include <atomic>
#include <thread>
#include <poll.h>

#include "ConnectionManager.hpp"
#include "MpdClientCache.hpp"

namespace ImpyD::Mpd
{
    class IdleClientWrapper
    {
    public:
        IdleClientWrapper(const char *host, int port);

        mpd_idle GetEventsAndClear();

    private:
        ConnectionManager connection;

        std::jthread idleThread;

        std::atomic_int queuedEvents;
        std::atomic_bool kill;

        void ThreadEntry(std::stop_token st);
    };
}

#endif //IM_MPD_IDLECLIENTWRAPPER_HPP