#ifndef IM_MPD_MPDSONGWRAPPER_HPP
#define IM_MPD_MPDSONGWRAPPER_HPP

#include <mpd/song.h>
#include "../TitleFormatting/ITagged.hpp"

//Manages the lifetime of a mpd_song and provides some utility methods.
class MpdSongWrapper : public ImpyD::TitleFormatting::ITagged
{
    std::shared_ptr<mpd_song> song = nullptr;

public:
    MpdSongWrapper(mpd_song *song);

    MpdSongWrapper &operator=(MpdSongWrapper rhs)
    {
        song = std::move(rhs.song);
        return *this;
    }

    [[nodiscard]] std::string GetSingleValue(mpd_tag_type key) const override;

    [[nodiscard]] std::vector<std::string> GetAllValues(mpd_tag_type key) const override;

    [[nodiscard]] unsigned GetId() const override;

    [[nodiscard]] unsigned GetDurationMs() const override;

    [[nodiscard]] std::string GetUri() const override;

    [[nodiscard]] std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> GetFilters() const override;
};


#endif //IM_MPD_MPDSONGWRAPPER_HPP