//
// Created by aiden on 12/24/25.
//

#ifndef IM_MPD_TAGFILTERGENERATOR_HPP
#define IM_MPD_TAGFILTERGENERATOR_HPP
#include <string>
#include <utility>

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

        TagFilterGenerator(mpd_tag_type tag, std::string value): tag(tag), value(std::move(value))
        {
        }

        TagFilterGenerator(const std::pair<mpd_tag_type, std::string> &pair) : TagFilterGenerator(pair.first, pair.second)
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