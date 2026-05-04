#ifndef IM_MPD_FINDACTION_HPP
#define IM_MPD_FINDACTION_HPP
#include <vector>

#include "PromiseAction.hpp"
#include "../MpdSongWrapper.hpp"

namespace ImpyD::ClientActions
{
    class FindAction : public PromiseAction<std::vector<std::unique_ptr<TitleFormatting::ITagged>>>
    {
        const std::vector<std::unique_ptr<Mpd::IFilterGenerator>> filters;
        const mpd_tag_type sort;
        const bool addToQueue;

        void SetupFind(mpd_connection *connection) const;

    public:
        FindAction(std::vector<std::unique_ptr<Mpd::IFilterGenerator>> filters, mpd_tag_type sort, bool addToQueue);

    protected:
        std::vector<std::unique_ptr<TitleFormatting::ITagged>> GetValue(mpd_connection *connection) override;
    };
}

#endif //IM_MPD_FINDACTION_HPP
