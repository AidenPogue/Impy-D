#ifndef IM_MPD_MPDFILTERGENERATOR_HPP
#define IM_MPD_MPDFILTERGENERATOR_HPP
#include <mpd/client.h>


namespace ImpyD::Mpd
{
    class IFilterGenerator
    {
    public:
        virtual ~IFilterGenerator() = default;

        virtual void ApplyFilter(mpd_connection *connection) = 0;
    };
}

#endif //IM_MPD_MPDFILTERGENERATOR_HPP