#include "IdleClientWrapper.hpp"

#include <iostream>

namespace ImpyD::Mpd
{
    IdleClientWrapper::IdleClientWrapper(const char *host, unsigned port):
        connection(host, port),
        idleThread([this](std::stop_token st) { ThreadEntry(st); })
    {
    }

    mpd_idle IdleClientWrapper::GetEventsAndClear()
    {
        auto val = queuedEvents.load();
        queuedEvents.store(0);
        return static_cast<mpd_idle>(val);
    }

    void IdleClientWrapper::ThreadEntry(std::stop_token st)
    {
        auto conn = connection.GetConnection();
        mpd_send_idle(conn);

        struct pollfd pfd;
        pfd.fd = mpd_connection_get_fd(conn);
        pfd.events = POLLIN;   // data available to read
        pfd.revents = 0;

        while (!st.stop_requested())
        {
            int ret = poll(&pfd, 1, -1);

            if (ret > 0 && pfd.revents & (POLLIN | POLLERR | POLLHUP))
            {
                auto val = mpd_recv_idle(conn, false);
                std::cout << "Thread: " << mpd_idle_name(val) << std::endl;
                queuedEvents.fetch_or(val);
            }

            mpd_send_idle(conn);
        }

        std::cout << "Exiting idle thread." << std::endl;
    }
} // ImpyD