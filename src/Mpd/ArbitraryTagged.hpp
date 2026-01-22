#ifndef IM_MPD_LISTRESULT_HPP
#define IM_MPD_LISTRESULT_HPP
#include <unordered_map>
#include <mpd/tag.h>

#include "TagFilterGenerator.hpp"
#include "../TitleFormatting/ITagged.hpp"

namespace ImpyD::Mpd
{
    /**
     * ITaggable that wraps a single string -> vector<string> map.
     */
    class ArbitraryTagged : public TitleFormatting::ITagged
    {
        std::unordered_map<mpd_tag_type, std::vector<std::string>> map;

    public:

        void AddValue(mpd_tag_type key, std::string value);

        std::string GetSingleValue(mpd_tag_type key) const override;

        std::vector<std::string> GetAllValues(mpd_tag_type key) const override;

        std::vector<std::unique_ptr<IFilterGenerator>> GetFilters() const override;
    };
}

#endif //IM_MPD_LISTRESULT_HPP