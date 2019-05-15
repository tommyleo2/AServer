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

  protected:
    struct PrivateConstruct {};

  public:
    TimerHandle(const PrivateConstruct &, boost::asio::io_context &io,
                Timer &timer, float wait_time, function<void()> &&callback);

    virtual ~TimerHandle() = default;

    void call();
    void cancel();

  protected:
    virtual bool onTimer(const boost::system::error_code &e) = 0;

  protected:
    boost::asio::io_context &m_io;

    chrono::milliseconds m_wait_time;
    function<void()> m_bound_callback;
    function<void(const boost::system::error_code &)> m_callback_wrapper;

    boost::asio::steady_timer m_timer_handle;

    Timer &m_timer;
};

class TimerHandle_Once : public TimerHandle {
    friend Timer;

  public:
    TimerHandle_Once(const PrivateConstruct &, boost::asio::io_context &io,
                     Timer &timer, float wait_time,
                     function<void()> &&callback);

  protected:
    template <typename Function, typename... ArgTypes>
    static shared_ptr<TimerHandle> create(boost::asio::io_context &io,
                                          Timer &timer, float wait_time,
                                          Function &&callback,
                                          ArgTypes &&... args) {
        return std::make_shared<TimerHandle_Once>(
            PrivateConstruct{}, io, timer, wait_time,
            std::bind(std::forward<Function>(callback),
                      std::forward<ArgTypes>(args)...));
    }
    bool onTimer(const boost::system::error_code &e) override;
};

class TimerHandle_Repeat : public TimerHandle {
    friend Timer;

  public:
    TimerHandle_Repeat(const PrivateConstruct &, boost::asio::io_context &io,
                       Timer &timer, float wait_time,
                       function<void()> &&callback);

  protected:
    template <typename Function, typename... ArgTypes>
    static shared_ptr<TimerHandle> create(boost::asio::io_context &io,
                                          Timer &timer, float wait_time,
                                          Function &&callback,
                                          ArgTypes &&... args) {
        return std::make_shared<TimerHandle_Repeat>(
            PrivateConstruct{}, io, timer, wait_time,
            std::bind(std::forward<Function>(callback),
                      std::forward<ArgTypes>(args)...));
    }

    bool onTimer(const boost::system::error_code &e) override;
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
        auto handle = TimerHandle_Once::create(m_io, *this, wait_time,
                                               std::forward<Function>(callback),
                                               std::forward<ArgTypes>(args)...);
        weak_ptr<TimerHandle> ret(handle);
        m_handles.emplace(std::move(handle));
        return ret;
    }

    template <typename Function, typename... ArgTypes>
    weak_ptr<TimerHandle> repeat(float wait_time, Function &&callback,
                                 ArgTypes &&... args) {
        auto handle = TimerHandle_Repeat::create(
            m_io, *this, wait_time, std::forward<Function>(callback),
            std::forward<ArgTypes>(args)...);
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
