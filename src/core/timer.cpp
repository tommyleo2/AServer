//
// Created by tommy on 4/20/19.
//
#include "core/timer.hpp"

using namespace AServer::Core;
using namespace boost::asio;

using error_code = boost::system::error_code;

TimerHandle::TimerHandle(const PrivateConstruct &, io_context &io, Timer &timer,
                         float wait_time, AServer::function<void()> &&callback)
    : m_io(io),
      m_wait_time(static_cast<decltype(m_wait_time)::rep>(wait_time * 1000)),
      m_bound_callback(std::move(callback)),
      m_callback_wrapper([this](const error_code &e) {
          if (this->onTimer(e)) {
              m_timer.onTimerEnded(shared_from_this());
          }
      }),
      m_timer_handle(m_io, m_wait_time),
      m_timer(timer) {
    m_timer_handle.async_wait(m_callback_wrapper);
}

void TimerHandle::call() {
    m_bound_callback();
}

void TimerHandle::cancel() {
    m_timer_handle.cancel();
}

TimerHandle_Once::TimerHandle_Once(const TimerHandle::PrivateConstruct &cp,
                                   boost::asio::io_context &io, Timer &timer,
                                   float wait_time,
                                   AServer::function<void()> &&callback)
    : TimerHandle(cp, io, timer, wait_time, std::move(callback)) {}

bool TimerHandle_Once::onTimer(const error_code &e) {
    if (e) {
        if (e.value() != ECANCELED) {
            // TODO: throw
            std::cout << e.message() << std::endl;
        }
        return true;
    }
    this->call();
    return true;
}

TimerHandle_Repeat::TimerHandle_Repeat(const TimerHandle::PrivateConstruct &cp,
                                       boost::asio::io_context &io,
                                       Timer &timer, float wait_time,
                                       AServer::function<void()> &&callback)
    : TimerHandle(cp, io, timer, wait_time, std::move(callback)) {}

bool TimerHandle_Repeat::onTimer(const error_code &e) {
    if (e) {
        if (e.value() != ECANCELED) {
            // TODO: throw
            std::cout << e.message() << std::endl;
        }
        return true;
    }
    this->call();

    // FIXME: Use expiry time or current time ?
    m_timer_handle.expires_at(m_timer_handle.expiry() + m_wait_time);
    m_timer_handle.async_wait(m_callback_wrapper);
    return false;
}

Timer::Timer(const PrivateConstruct &, boost::asio::io_context &io)
    : m_io(io) {}

Timer::~Timer() {
    assert(m_handles.empty());
}

AServer::shared_ptr<Timer> Timer::create(boost::asio::io_context &io) {
    return std::make_shared<Timer>(PrivateConstruct{}, io);
}

void Timer::onTimerEnded(const shared_ptr<TimerHandle> &handle) {
    m_handles.erase(handle);
}
