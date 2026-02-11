#ifndef IM_MPD_CLIENTMANAGER_HPP
#define IM_MPD_CLIENTMANAGER_HPP
#include <memory>
#include <mpd/connection.h>

namespace ImpyD::Mpd
{
    class ConnectionManager
    {
        mpd_connection *connection = nullptr;

        const char *host;
        unsigned port;

        void Connect();

        public:
        ConnectionManager(const char *host, unsigned port);

        /**
         * Creates a new connection using the host and port of an existing one.
         * @param existing
         */
        explicit ConnectionManager(const ConnectionManager &existing);

        ~ConnectionManager();

        bool CheckConnected() const;

        mpd_connection *GetConnection();
    };
}



#endif //IM_MPD_CLIENTMANAGER_HPP