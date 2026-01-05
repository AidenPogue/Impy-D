//
// Created by aiden on 12/24/25.
//

#include "TagFilterGenerator.hpp"

namespace ImpyD::Mpd
{
    void TagFilterGenerator::ApplyFilter(mpd_connection *connection)
    {
        mpd_search_add_tag_constraint(connection, MPD_OPERATOR_DEFAULT, tag, value.c_str());
    }
} // ImpyD