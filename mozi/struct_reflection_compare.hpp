/*
 * Copyright (c) 2023-2024 Wu Yongwei
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#ifndef MOZI_STRUCT_REFLECTION_COMPARE_HPP
#define MOZI_STRUCT_REFLECTION_COMPARE_HPP

#include "metamacro.h"                 // MOZI_DO_PRAGMA
#include "compare.hpp"                 // MOZI_USE_THREE_WAY_COMPARISON/...
#include "struct_reflection_core.hpp"  // MOZI_DECLARE_LESS_COMPARISON/...
#include "struct_reflection_equal.hpp" // MOZI_DECLARE_EQUAL_COMPARISON

#if MOZI_USE_THREE_WAY_COMPARISON

#include <algorithm>                   // std::min
#include <compare>                     // std::strong_ordering
#include <cstddef>                     // std::size_t
#include <type_traits>                 // std::remove_cvref
#include <utility>                     // std::forward/index_sequence/...
#include "type_traits.hpp"             // mozi::is_reflected_struct

namespace mozi {

namespace detail {

template <typename T, typename U, std::size_t... Is,
          std::enable_if_t<(mozi::is_reflected_struct_v<T> &&
                            mozi::is_reflected_struct_v<U>),
                           int> = 0>
constexpr auto get_common_comparison_type_impl(std::index_sequence<Is...>)
{
    return mozi::type_t<std::common_type_t<
        decltype(std::declval<
                     typename T::template _field<T, Is>::type>() <=>
                 std::declval<
                     typename U::template _field<T, Is>::type>())...>>{};
}

} // namespace detail

template <typename T, typename U = T,
          std::enable_if_t<(mozi::is_reflected_struct_v<T> &&
                            mozi::is_reflected_struct_v<U>),
                           int> = 0>
constexpr auto get_common_comparison_type()
{
    return detail::get_common_comparison_type_impl<T, U>(
        std::make_index_sequence<std::min(T::_size, U::_size)>{});
}

template <typename T, typename U>
struct comparer<T, U,
                std::enable_if_t<is_reflected_struct_v<T> &&
                                 is_reflected_struct_v<U>>> {
    template <typename T1, typename U1>
    constexpr auto operator()(T1&& lhs, U1&& rhs) const
    {
        static_assert(std::is_same_v<std::remove_cvref_t<T1>, T> &&
                      std::is_same_v<std::remove_cvref_t<U1>, U>);
        typename decltype(get_common_comparison_type<T, U>())::type result =
            std::strong_ordering::equivalent;
        zip(std::forward<T1>(lhs), std::forward<U1>(rhs),
            [&result](auto /*name1*/, auto /*name2*/,
                      auto&& value1, auto&& value2) {
                if (result == std::strong_ordering::equivalent) {
                    result =
                        compare(std::forward<decltype(value1)>(value1),
                                std::forward<decltype(value2)>(value2));
                }
            });
        if (result == std::strong_ordering::equivalent) {
            result = T::_size <=> U::_size;
        }
        return result;
    }
};

} // namespace mozi

#define MOZI_DECLARE_COMPARISON_IMP(st)                                    \
    MOZI_DECLARE_EQUAL_COMPARISON(st)                                      \
    constexpr auto operator<=>(const st& lhs, const st& rhs)               \
    {                                                                      \
        static_assert(mozi::is_reflected_struct_v<st>);                    \
        return mozi::compare(lhs, rhs);                                    \
    }

#else

#define MOZI_DECLARE_COMPARISON_IMP(st)                                    \
    MOZI_DECLARE_EQUAL_COMPARISON(st)                                      \
    MOZI_DECLARE_LESS_COMPARISON(st)                                       \
    constexpr bool operator!=(const st& lhs, const st& rhs)                \
    {                                                                      \
        return !(rhs == lhs);                                              \
    }                                                                      \
    constexpr bool operator>(const st& lhs, const st& rhs)                 \
    {                                                                      \
        return (rhs < lhs);                                                \
    }                                                                      \
    constexpr bool operator<=(const st& lhs, const st& rhs)                \
    {                                                                      \
        return !(rhs < lhs);                                               \
    }                                                                      \
    constexpr bool operator>=(const st& lhs, const st& rhs)                \
    {                                                                      \
        return !(lhs < rhs);                                               \
    }

#endif

#if defined(__clang__)
#define MOZI_DECLARE_COMPARISON(st)                                        \
    MOZI_DO_PRAGMA(GCC diagnostic push)                                    \
    MOZI_DO_PRAGMA(GCC diagnostic ignored "-Wunused-function")             \
    MOZI_DECLARE_COMPARISON_IMP(st)                                        \
    MOZI_DO_PRAGMA(GCC diagnostic pop)
#else
#define MOZI_DECLARE_COMPARISON MOZI_DECLARE_COMPARISON_IMP
#endif

#if !defined(DECLARE_COMPARISON) && !defined(MOZI_NO_DECLARE_COMPARISON)
#define DECLARE_COMPARISON MOZI_DECLARE_COMPARISON
#endif

#endif // MOZI_STRUCT_REFLECTION_COMPARE_HPP
