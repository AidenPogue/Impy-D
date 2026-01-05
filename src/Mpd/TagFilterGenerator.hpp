//
// Created by aiden on 12/24/25.
//

#ifndef IM_MPD_TAGFILTERGENERATOR_HPP
#define IM_MPD_TAGFILTERGENERATOR_HPP
#include <string>

#include "IFilterGenerator.hpp"

namespace ImpyD::Mpd
{
    class TagFilterGenerator : public IFilterGenerator
    {
    public:
        enum class Operator
        {
            Match,
            NoMatch,
            StartsWith,
            EndsWith,
        };

        TagFilterGenerator(const mpd_tag_type tag, const std::string value)
            : tag(tag),
              value(value)
        {
        }

        void ApplyFilter(mpd_connection *connection) override;

    private:
        const mpd_tag_type tag;
        const Operator op = Operator::Match;
        const std::string value;
    };
} // ImpyD

#endif //IM_MPD_TAGFILTERGENERATOR_HPP