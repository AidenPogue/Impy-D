#ifndef IM_MPD_PROMISEACTION_HPP
#define IM_MPD_PROMISEACTION_HPP
#include <future>

#include "IClientAction.hpp"

namespace ImpyD::ClientActions
{
    template<class ReturnType>
    class PromiseAction : public IClientAction
    {
        std::promise<ReturnType> promise;

    protected:
        virtual ReturnType GetValue(mpd_connection *connection) = 0;
    public:
        std::future<ReturnType> GetFuture()
        {
            return promise.get_future();
        }

        void Execute(mpd_connection *connection) override
        {
            promise.set_value(GetValue(connection));
        }
    };
}

#endif //IM_MPD_PROMISEACTION_HPP
