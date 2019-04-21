//
// Created by tommy on 4/21/19.
//

#ifndef ASERVER_WEAK_CALL_HPP
#define ASERVER_WEAK_CALL_HPP

#include "core/define.hpp"

namespace AServer::Core {
class WeakCallable {
  public:
    template <typename ActualClass, typename MemberFunction,
              typename... ArgTypes>
    static WeakCallable create(weak_ptr<ActualClass> &&callee_object,
                               MemberFunction &&member_func,
                               ArgTypes &&... args) {
        //  Perfect capturing is available in c++20
        //  ugly workaround
        auto call_impl =
            [callee_object{std::move(callee_object)},
             member_func = mem_fn(member_func),
             args = make_tuple(std::forward<ArgTypes>(args)...)]() mutable {
                auto concrete = callee_object.lock();
                if (concrete) {
                    std::apply(
                        [&](auto &&... args) {
                            member_func(concrete,
                                        std::forward<decltype(args)>(args)...);
                        },
                        std::move(args));
                }
            };
        return WeakCallable(std::move(call_impl));
    }

    inline void operator()() {
        m_call_impl();
    }

  private:
    explicit WeakCallable(function<void()> &&call_impl)
        : m_call_impl(std::move(call_impl)) {}

  private:
    function<void()> m_call_impl;
};

template <typename SelfClass>
class WeakCallee : public enable_shared_from_this<SelfClass> {
  public:
    template <typename MemberFunction, typename... ArgTypes>
    WeakCallable makeWeakCallable(MemberFunction &&func, ArgTypes &&... args) {
        return WeakCallable::create(this->weak_from_this(), func,
                                    std::forward<ArgTypes>(args)...);
    }
};
}  // namespace AServer::Core

#endif  // ASERVER_WEAK_CALL_HPP
