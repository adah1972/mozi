/*
 * Copyright (c) 2024-2025 Wu Yongwei
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

#ifndef MOZI_SPAN_HPP
#define MOZI_SPAN_HPP

#define MOZI_SPAN_GSL 1
#define MOZI_SPAN_STD 2

#ifndef MOZI_USE_SPAN
#if __cplusplus > 201703L && __has_include(<span>)
#define MOZI_USE_SPAN MOZI_SPAN_STD
#elif __has_include(<gsl/span>)
#define MOZI_USE_SPAN MOZI_SPAN_GSL
#endif
#endif

#if MOZI_USE_SPAN == MOZI_SPAN_STD
#include <span>        // IWYU pragma: export std::span
#elif MOZI_USE_SPAN == MOZI_SPAN_GSL
#include <gsl/span>    // IWYU pragma: export gsl::span
#else
#error "No span support is detected"
#endif

#include <type_traits> // std::is_same/remove_cv
#include "equal.hpp"   // mozi::equal

namespace mozi {

#if MOZI_USE_SPAN == MOZI_SPAN_STD
using std::span;
#else
using gsl::span;
#endif

template <typename T, typename U>
struct equality_comparer<span<T>, span<U>, void> {
    template <typename T1, typename U1>
    constexpr bool operator()(span<T1> lhs, span<U1> rhs) const
    {
        static_assert(
            std::is_same_v<std::remove_cv_t<T1>, std::remove_cv_t<T>> &&
            std::is_same_v<std::remove_cv_t<U1>, std::remove_cv_t<U>>);
        if (lhs.size() != rhs.size()) {
            return false;
        }
        auto it1 = lhs.begin();
        auto it2 = rhs.begin();
        while (it1 != lhs.end()) {
            if (*it1 != *it2) {
                return false;
            }
            ++it1;
            ++it2;
        }
        return true;
    }
};

} // namespace mozi

#endif // MOZI_SPAN_HPP
