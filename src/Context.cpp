#include "Context.hpp"

namespace ImpyD
{
    Context::Context(const char *host, unsigned port) : mainClient(host, port), idleClient(host, port), config(LoadConfigFromDisk())
    {

    }

    MpdClientWrapper &Context::GetClient()
    {
        return mainClient;
    }

    Mpd::IdleClientWrapper & Context::GetIdleClient()
    {
        return idleClient;
    }

    Config & Context::GetConfig()
    {
        return config;
    }
}
