#ifndef IM_MPD_LISTACTION_HPP
#define IM_MPD_LISTACTION_HPP
#include <vector>

#include "../ArbitraryTagged.hpp"
#include "PromiseAction.hpp"

namespace ImpyD::ClientActions
{
    class ListAction : public PromiseAction<std::vector<std::unique_ptr<TitleFormatting::ITagged>>>
    {
    public:
        ListAction(std::vector<mpd_tag_type> groups, std::vector<std::unique_ptr<Mpd::IFilterGenerator>> filters = {});

    private:
        const std::vector<mpd_tag_type> groups;
        const std::vector<std::unique_ptr<Mpd::IFilterGenerator>> filters;

    protected:
        std::vector<std::unique_ptr<TitleFormatting::ITagged>> GetValue(mpd_connection *connection) override;

    };
}

#endif //IM_MPD_LISTACTION_HPP
