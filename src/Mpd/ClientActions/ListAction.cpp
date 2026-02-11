#include "ListAction.hpp"

#include <cassert>

namespace ImpyD::ClientActions
{
    ListAction::ListAction(std::vector<mpd_tag_type> groups,
        std::vector<std::unique_ptr<Mpd::IFilterGenerator>> filters): groups(std::move(groups)),
                                                                      filters(std::move(filters))
    {
    }

    std::vector<std::unique_ptr<TitleFormatting::ITagged>> ListAction::GetValue(mpd_connection *connection)
    {
        mpd_search_cancel(connection);

        assert(!groups.empty());
        mpd_search_db_tags(connection, groups.back());

        if (!filters.empty())
        {
            for (auto &expression : filters)
            {
                expression->ApplyFilter(connection);
            }
        }

        for (int i = groups.size() - 2; i >= 0; i--)
        {
            const auto &group = groups.at(i);
            mpd_search_add_group_tag(connection, group);
        }

        mpd_search_commit(connection);

        std::vector<std::unique_ptr<TitleFormatting::ITagged>> list;

        //TODO: explain what this is for
        std::vector<std::pair<mpd_tag_type, std::string>> tagStack;

        auto pair = mpd_recv_pair(connection);
        while (pair != nullptr)
        {
            auto currentType = mpd_tag_name_iparse(pair->name);

            if (groups.at(tagStack.size()) == currentType)
            {
                tagStack.emplace_back(std::pair(currentType, pair->value));
            }
            else
            {
                while (tagStack.back().first != currentType)
                {
                    tagStack.pop_back();
                }
                tagStack.pop_back();
                tagStack.emplace_back(std::pair(currentType, pair->value));
            }

            if (currentType == groups.back())
            {
                auto item = new Mpd::ArbitraryTagged();

                for (const auto &[fst, snd] : tagStack)
                {
                    item->AddValue(fst, snd);
                }

                tagStack.pop_back();
                list.push_back(std::unique_ptr<Mpd::ArbitraryTagged>(item));
            }

            mpd_return_pair(connection, pair);
            pair = mpd_recv_pair(connection);
        }

        return list;
    }
}
