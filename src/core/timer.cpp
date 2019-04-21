//
// Created by tommy on 4/20/19.
//
#include "core/timer.hpp"

using namespace AServer::Core;
using namespace boost::asio;

using error_code = boost::system::error_code;

TimerHandle::TimerHandle(const PrivateConstruct &, io_context &io,
                         weak_ptr<Timer> &&timer_ref, bool is_repetitive,
                         float wait_time, AServer::function<void()> &&callback)
    : m_io(io),
      m_timer_ref(std::move(timer_ref)),
      m_is_repetitive(is_repetitive),
      m_wait_time(static_cast<decltype(m_wait_time)::rep>(wait_time * 1000)),
      m_callback(callback),
      m_callback_wrapper([this](const error_code &e) { this->onTriggered(e); }),
      m_timer_handle(m_io, m_wait_time) {
    m_timer_handle.async_wait(m_callback_wrapper);
}

void TimerHandle::onTriggered(const error_code &e) {
    if (e) {
        // We must possess a ref to self,
        // so that it wont be destructed within this block
        auto self = shared_from_this();
        m_timer_ref.lock()->onTimerEnded(self);

        if (e.value() != ECANCELED) {
            // TODO: throw
            std::cout << e.message() << std::endl;
        }
        return;
    }
    m_callback();
    if (m_is_repetitive) {
        m_timer_handle.expires_at(m_timer_handle.expiry() + m_wait_time);
        m_timer_handle.async_wait(m_callback_wrapper);
    } else {
        // We must possess a ref to self,
        // so that it wont be destructed within this block
        auto self = shared_from_this();
        m_timer_ref.lock()->onTimerEnded(self);
    }
}

void TimerHandle::cancel() {
    this->m_timer_handle.cancel();
}

Timer::Timer(const PrivateConstruct &, boost::asio::io_context &io)
    : m_io(io) {}

Timer::~Timer() {
    assert(m_handles.empty());
}

AServer::shared_ptr<Timer> Timer::create(boost::asio::io_context &io) {
    return make_shared<Timer>(PrivateConstruct{}, io);
}

void Timer::onTimerEnded(const shared_ptr<TimerHandle> &handle) {
    m_handles.erase(handle);
}
