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

#ifndef MOZI_EQUAL_HPP
#define MOZI_EQUAL_HPP

#include <functional>      // std::equal_to
#include <type_traits>     // std::is_same
#include <utility>         // std::forward
#include "type_traits.hpp" // mozi::remove_cvref

namespace mozi {

template <typename T, typename U, typename = void>
struct equality_comparer {
    template <typename T1, typename U1>
    constexpr bool operator()(T1&& lhs, U1&& rhs) const
    {
        static_assert(std::is_same_v<mozi::remove_cvref_t<T1>, T> &&
                      std::is_same_v<mozi::remove_cvref_t<U1>, U>);
        return std::equal_to<>{}(std::forward<T1>(lhs),
                                 std::forward<U1>(rhs));
    }
};

namespace detail {

struct equal_fn {
    template <typename T, typename U>
    constexpr bool operator()(T&& lhs, U&& rhs) const
    {
        return equality_comparer<mozi::remove_cvref_t<T>,
                                 mozi::remove_cvref_t<U>>{}(
            std::forward<T>(lhs), std::forward<U>(rhs));
    }
};

} // namespace detail

inline constexpr detail::equal_fn equal{};

} // namespace mozi

#endif // MOZI_EQUAL_HPP
