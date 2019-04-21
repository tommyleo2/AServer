//
// Created by tommy on 4/20/19.
//

#ifndef ASERVER_DEFINE_HPP
#define ASERVER_DEFINE_HPP

#include <deque>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <chrono>
#include <functional>
#include <memory>

#include <iostream>

namespace AServer {
namespace chrono = std::chrono;

#define USING_STD_TYPE(type) \
    template <typename... T> \
    using type = std::type<T...>;

#define USING_STD_TYPE_FUNC_1(func)                                 \
    template <typename T, typename... ArgsType>                     \
    inline auto func(ArgsType&&... args)                            \
        ->decltype(std::func<T>(std::forward<ArgsType>(args)...)) { \
        return std::func<T>(std::forward<ArgsType>(args)...);       \
    }

USING_STD_TYPE(function)
USING_STD_TYPE(unique_ptr)
USING_STD_TYPE(shared_ptr)
USING_STD_TYPE(weak_ptr)
USING_STD_TYPE(vector)
USING_STD_TYPE(deque)
USING_STD_TYPE(set)
USING_STD_TYPE(unordered_set)
USING_STD_TYPE(enable_shared_from_this)

USING_STD_TYPE(map)
USING_STD_TYPE(unordered_map)

USING_STD_TYPE_FUNC_1(make_unique)
USING_STD_TYPE_FUNC_1(make_shared)

}  // namespace AServer

#endif  // ASERVER_DEFINE_HPP
