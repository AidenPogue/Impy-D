#include "FindAction.hpp"

#include "../../Utils.hpp"

namespace ImpyD::ClientActions
{
    void FindAction::SetupFind(mpd_connection *connection) const
    {
        if (filters.empty())
        {
            //Fight me, MPD docs. I WILL keep a copy of the entire database
            mpd_search_add_expression(connection, "(any != '')");
        }
        else
        {
            for (auto &expression : filters)
            {
                expression->ApplyFilter(connection);
            }
        }

        if (sort != MPD_TAG_UNKNOWN)
        {
            mpd_search_add_sort_tag(connection, sort, false);
        }
    }

    FindAction::FindAction(std::vector<std::unique_ptr<Mpd::IFilterGenerator>> filters, mpd_tag_type sort, bool addToQueue):
        filters(std::move(filters)),
        sort(sort),
        addToQueue(addToQueue)
    {
    }

    std::vector<std::unique_ptr<TitleFormatting::ITagged>> FindAction::GetValue(mpd_connection *connection)
    {
        mpd_search_cancel(connection);

        if (addToQueue)
        {
            mpd_search_add_db_songs(connection, true);

            SetupFind(connection);

            mpd_search_commit(connection);
            mpd_response_finish(connection);
            //This is rather unelegant that we have to return something here,
            //but given how much code add vs. get share it seemed natural to combine them like this.
            return {};
        }
        else
        {
            mpd_search_db_songs(connection, true);

            SetupFind(connection);

            mpd_search_commit(connection);

            return Utils::ReceiveSongList(connection);
        }
    }
}
