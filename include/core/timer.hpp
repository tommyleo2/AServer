//
// Created by tommy on 4/20/19.
//

#ifndef ASERVER_TIMER_HPP
#define ASERVER_TIMER_HPP

#include <cerrno>
#include "boost/asio.hpp"

#include "core/define.hpp"

namespace AServer::Core {
class IOServer;
class Timer;

class TimerHandle : public enable_shared_from_this<TimerHandle> {
    friend Timer;

  private:
    struct PrivateConstruct {};

  public:
    TimerHandle(const PrivateConstruct &, boost::asio::io_context &io,
                weak_ptr<Timer> &&timer_ref, bool is_repetitive,
                float wait_time, function<void()> &&callback);

    void cancel();

  private:
    template <typename Function, typename... ArgTypes>
    static shared_ptr<TimerHandle> create(boost::asio::io_context &io,
                                          weak_ptr<Timer> &&timer_ref,
                                          bool is_repetitive, float wait_time,
                                          Function &&callback,
                                          ArgTypes &&... args) {
        return std::make_shared<TimerHandle>(
            PrivateConstruct{}, io, std::move(timer_ref), is_repetitive,
            wait_time,
            std::bind(std::forward<Function>(callback),
                      std::forward<ArgTypes>(args)...));
    }

    void onTriggered(const boost::system::error_code &e);

  private:
    boost::asio::io_context &m_io;

    bool m_is_repetitive;
    chrono::milliseconds m_wait_time;
    function<void()> m_callback;
    function<void(const boost::system::error_code &)> m_callback_wrapper;

    boost::asio::steady_timer m_timer_handle;

    weak_ptr<Timer> m_timer_ref;
};

class Timer : public enable_shared_from_this<Timer> {
    friend IOServer;
    friend TimerHandle;

  private:
    struct PrivateConstruct {};

  public:
    Timer(const PrivateConstruct &, boost::asio::io_context &io);
    ~Timer();

    template <typename Function, typename... ArgTypes>
    weak_ptr<TimerHandle> once(float wait_time, Function &&callback,
                               ArgTypes &&... args) {
        auto handle = TimerHandle::create(
            m_io, weak_from_this(), false, wait_time,
            std::forward<Function>(callback), std::forward<ArgTypes>(args)...);
        weak_ptr<TimerHandle> ret(handle);
        m_handles.emplace(std::move(handle));
        return ret;
    }

    template <typename Function, typename... ArgTypes>
    weak_ptr<TimerHandle> repeat(float wait_time, Function &&callback,
                                 ArgTypes &&... args) {
        auto handle = TimerHandle::create(
            m_io, weak_from_this(), true, wait_time,
            std::forward<Function>(callback), std::forward<ArgTypes>(args)...);
        weak_ptr<TimerHandle> ret(handle);
        m_handles.emplace(std::move(handle));
        return ret;
    }

  private:
    static shared_ptr<Timer> create(boost::asio::io_context &io);

    void onTimerEnded(const shared_ptr<TimerHandle> &handle);

  private:
    unordered_set<shared_ptr<TimerHandle>> m_handles;
    boost::asio::io_context &m_io;
};
}  // namespace AServer::Core

#endif  // ASERVER_TIMER_HPP
