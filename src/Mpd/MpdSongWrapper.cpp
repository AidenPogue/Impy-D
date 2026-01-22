#include "MpdSongWrapper.hpp"

#include <assert.h>
#include <stdexcept>

#include "UriFilterGenerator.hpp"

MpdSongWrapper::MpdSongWrapper(mpd_song *song) : song(std::shared_ptr<mpd_song>(song, &mpd_song_free))
{
    assert(song);
}

std::string MpdSongWrapper::GetSingleValue(mpd_tag_type key) const
{
    auto val = mpd_song_get_tag(song.get(), key, 0);
    return val ? std::string(val) : std::string();
}

std::vector<std::string> MpdSongWrapper::GetAllValues(mpd_tag_type key) const
{
    auto list = std::vector<std::string>();
    //Most tags probably won't have more than 2 values.
    list.reserve(2);

    auto idx = 0;
    auto rawValue = mpd_song_get_tag(song.get(), key, idx);

    while (rawValue != nullptr)
    {
        list.emplace_back(rawValue);
        rawValue = mpd_song_get_tag(song.get(), key, ++idx);
    }

    return list;
}

unsigned MpdSongWrapper::GetId() const
{
    return mpd_song_get_id(song.get());
}

unsigned MpdSongWrapper::GetDurationMs() const
{
    return mpd_song_get_duration_ms(song.get());
}

std::string MpdSongWrapper::GetUri() const
{
    return mpd_song_get_uri(song.get());
}

std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>> MpdSongWrapper::GetFilters() const
{
    auto out = std::vector<std::unique_ptr<ImpyD::Mpd::IFilterGenerator>>();
    out.emplace_back(std::make_unique<UriFilterGenerator>(mpd_song_get_uri(song.get())));
    return out;
}
