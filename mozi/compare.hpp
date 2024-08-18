/*
 * Copyright (c) 2024 Wu Yongwei
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

#ifndef MOZI_COMPARE_HPP
#define MOZI_COMPARE_HPP

#ifndef MOZI_USE_THREE_WAY_COMPARISON
#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)
#include <version>     // IWYU pragma: keep
#endif

// If the standard library does not fully support three-way comparison,
// using components like array or string in reflected structs can be a
// problem.  So by default Mozi enables three-way comparison only when the
// standard library claims to support three-way comparison.
#if __cpp_lib_three_way_comparison >= 201907L
#define MOZI_USE_THREE_WAY_COMPARISON 1
#else
#define MOZI_USE_THREE_WAY_COMPARISON 0
#endif
#endif

#if MOZI_USE_THREE_WAY_COMPARISON

#include <compare>     // std::strong_ordering
#include <cstddef>     // std::size_t
#include <type_traits> // std::is_same/remove_cv/remove_cvref
#include <utility>     // std::forward

namespace mozi {

template <typename T, typename U, typename = void>
struct comparer {
    template <typename T1, typename U1>
    constexpr auto operator()(T1&& lhs, U1&& rhs) const
    {
        static_assert(std::is_same_v<std::remove_cvref_t<T1>, T> &&
                      std::is_same_v<std::remove_cvref_t<U1>, U>);
        return (std::forward<T1>(lhs) <=> std::forward<U1>(rhs));
    }
};

namespace detail {

struct compare_fn {
    template <typename T, typename U>
    constexpr auto operator()(T&& lhs, U&& rhs) const
    {
        return comparer<std::remove_cvref_t<T>, std::remove_cvref_t<U>>{}(
            std::forward<T>(lhs), std::forward<U>(rhs));
    }
};

} // namespace detail

inline constexpr detail::compare_fn compare{};

template <typename T, typename U, std::size_t N>
struct comparer<T[N], U[N]> {
    template <typename T1, typename U1>
    constexpr auto operator()(T1 (&lhs)[N], U1 (&rhs)[N]) const
    {
        static_assert(
            std::is_same_v<std::remove_cv_t<T1>, std::remove_cv_t<T>> &&
            std::is_same_v<std::remove_cv_t<U1>, std::remove_cv_t<U>>);
        for (std::size_t i = 0; i < N; ++i) {
            auto result = mozi::compare(lhs[i], rhs[i]);
            if (result != std::strong_ordering::equivalent) {
                return result;
            }
        }
        return std::strong_ordering::equivalent;
    }
};

} // namespace mozi

#endif // MOZI_USE_THREE_WAY_COMPARISON

#endif // MOZI_COMPARE_HPP
