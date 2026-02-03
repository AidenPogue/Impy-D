#include "IdleClientWrapper.hpp"

#include <iostream>

namespace ImpyD::Mpd
{
    IdleClientWrapper::IdleClientWrapper(const char *host, int port):
        host(host),
        port(port),
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
        connection = mpd_connection_new(this->host, this->port, 0);
        mpd_send_idle(connection);

        struct pollfd pfd;
        pfd.fd = mpd_connection_get_fd(connection);
        pfd.events = POLLIN;   // data available to read
        pfd.revents = 0;

        while (!st.stop_requested())
        {
            int ret = poll(&pfd, 1, -1);

            if (ret > 0 && pfd.revents & (POLLIN | POLLERR | POLLHUP))
            {
                auto val = mpd_recv_idle(connection, false);
                std::cout << "Thread: " << mpd_idle_name(val) << std::endl;
                queuedEvents.fetch_or(val);
            }

            mpd_send_idle(connection);
        }

        std::cout << "Exiting idle thread." << std::endl;
        mpd_connection_free(connection);
    }
} // ImpyD