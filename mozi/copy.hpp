/*
 * Copyright (c) 2023 Wu Yongwei
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

#ifndef MOZI_COPIER_HPP
#define MOZI_COPIER_HPP

#include <utility>         // std::forward/move
#include "type_traits.hpp" // mozi::remove_cvref

namespace mozi {

template <typename T, typename U, typename = void>
struct copier {
    constexpr void operator()(const T& src, U& dest) const
    {
        dest = src;
    }
    constexpr void operator()(T&& src, U& dest) const
    {
        dest = std::move(src);
    }
};

namespace detail {

struct copy_fn {
    template <typename T, typename U>
    constexpr void operator()(T&& src, U& dest) const
    {
        copier<mozi::remove_cvref_t<T>, mozi::remove_cvref_t<U>>{}(
            std::forward<T>(src), dest);
    }
};

} // namespace detail

inline constexpr detail::copy_fn copy{};

} // namespace mozi

#endif // MOZI_COPIER_HPP
