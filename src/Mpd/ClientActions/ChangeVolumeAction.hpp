#ifndef IM_MPD_CHANGEVOLUMEACTION_HPP
#define IM_MPD_CHANGEVOLUMEACTION_HPP
#include "IClientAction.hpp"

namespace ImpyD::ClientActions
{
    class ChangeVolumeAction : public IClientAction
    {
    public:
        enum class Mode
        {
            Set,
            ChangeBy
        };

        ChangeVolumeAction(int value, Mode mode);

        void Execute(mpd_connection *connection) override;

    private:
        const int value;
        const Mode mode;
    };
}

#endif //IM_MPD_CHANGEVOLUMEACTION_HPP
