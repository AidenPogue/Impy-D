#include "UriFilterGenerator.hpp"

void UriFilterGenerator::ApplyFilter(mpd_connection *connection)
{
    mpd_search_add_uri_constraint(connection, MPD_OPERATOR_DEFAULT, uri.c_str());
}
