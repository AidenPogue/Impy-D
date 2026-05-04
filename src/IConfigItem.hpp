#ifndef IM_MPD_ICONFIGITEM_HPP
#define IM_MPD_ICONFIGITEM_HPP
#include <string>

namespace ImpyD
{
    class IConfigItem
    {
    public:
        virtual ~IConfigItem() = default;

        virtual std::string GetDisplayName() = 0;

        /**
         * Draws ImGui controls that will edit this instance.
         * @return Whether anything was changed.
         */
        virtual bool DrawEditor() = 0;
    };
}

#endif