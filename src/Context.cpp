#include "Context.hpp"

namespace ImpyD
{
    Context::Context(const char *host, unsigned port) : mainClient(host, port), idleClient(host, port), config(LoadConfigFromDisk()), fifoReader(config.visualizer.bufferSize, config.visualizer.fifoPath)
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

    FifoReader & Context::GetFifoReader()
    {
        return fifoReader;
    }

    Config & Context::GetConfig()
    {
        return config;
    }
}
