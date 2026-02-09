#ifndef IM_MPD_NOPARAMETERPROMISEACTION_HPP
#define IM_MPD_NOPARAMETERPROMISEACTION_HPP
#include <cassert>
#include "PromiseAction.hpp"

namespace ImpyD::ClientActions
{
    template<class T>
    class NoParameterPromiseAction : public PromiseAction<T>
    {
        T (*func)(mpd_connection *connection) = nullptr;

    protected:
        T GetValue(mpd_connection *connection) override
        {
            assert(func);
            return func(connection);
        }

    public:
        NoParameterPromiseAction(T (*func)(mpd_connection *)) : func(func)
        {
        }
    };
}

#endif //IM_MPD_NOPARAMETERPROMISEACTION_HPP
