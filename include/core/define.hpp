//
// Created by tommy on 4/20/19.
//

#ifndef ASERVER_DEFINE_HPP
#define ASERVER_DEFINE_HPP

#include <chrono>
#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <iostream>

namespace AServer {
namespace chrono = std::chrono;

#define USING_STD_TYPE(type) \
    template <typename... T> \
    using type = std::type<T...>;

#define USING_STD_TYPE_FUNC_1(func)                                 \
    template <typename T, typename... ArgTypes>                     \
    inline auto func(ArgTypes&&... args)                            \
        ->decltype(std::func<T>(std::forward<ArgTypes>(args)...)) { \
        return std::func<T>(std::forward<ArgTypes>(args)...);       \
    }

#define USING_STD_TYPE_FUNC_0(func)                              \
    template <typename... ArgTypes>                              \
    inline auto func(ArgTypes&&... args)                         \
        ->decltype(std::func(std::forward<ArgTypes>(args)...)) { \
        return std::func(std::forward<ArgTypes>(args)...);       \
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
USING_STD_TYPE(tuple)

// USING_STD_TYPE_FUNC_1(make_unique)
// USING_STD_TYPE_FUNC_1(make_shared)
// USING_STD_TYPE_FUNC_0(make_tuple)
// USING_STD_TYPE_FUNC_0(mem_fn)

}  // namespace AServer

#endif  // ASERVER_DEFINE_HPP
