//
// Created by aiden on 2026-02-03.
//

#include "ConnectionManager.hpp"

#include <assert.h>
#include <iostream>

void ImpyD::Mpd::ConnectionManager::Connect()
{
    if (connection)
    {
        mpd_connection_free(connection);
    }

    connection = mpd_connection_new(host, port, 0);
}

ImpyD::Mpd::ConnectionManager::ConnectionManager(const char *host, unsigned port)
{
    this->host = host;
    this->port = port;

    Connect();
}

ImpyD::Mpd::ConnectionManager::ConnectionManager(const ConnectionManager &existing) : ConnectionManager(existing.host, existing.port)
{
}

ImpyD::Mpd::ConnectionManager::~ConnectionManager()
{
    if (connection)
    {
        mpd_connection_free(connection);
    }
}

bool ImpyD::Mpd::ConnectionManager::CheckConnected() const
{
    if (!connection)
    {
        return false;
    }

    auto err = mpd_connection_get_error(connection);
    return err == MPD_ERROR_SUCCESS;
}

mpd_connection *ImpyD::Mpd::ConnectionManager::GetConnection()
{
    //Only one retry, kinda naive.
    //How should we do this?
    if (!CheckConnected())
    {
        std::cout << "Trying to reconnect client..." << std::endl;
        Connect();
    }

    return connection;
}
