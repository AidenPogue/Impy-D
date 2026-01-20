#ifndef IM_MPD_URIFILTERGENERATOR_HPP
#define IM_MPD_URIFILTERGENERATOR_HPP
#include <string>

#include "IFilterGenerator.hpp"


class UriFilterGenerator : public ImpyD::Mpd::IFilterGenerator
{
private:
    const std::string uri;

public:
    explicit UriFilterGenerator(std::string uri) : uri(std::move(uri))
        {
        }

    void ApplyFilter(mpd_connection *connection) override;
};

#endif //IM_MPD_URIFILTERGENERATOR_HPP