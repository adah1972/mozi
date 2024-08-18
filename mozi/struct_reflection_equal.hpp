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

#ifndef MOZI_STRUCT_REFLECTION_EQUAL_HPP
#define MOZI_STRUCT_REFLECTION_EQUAL_HPP

#include <type_traits>                // std::enable_if
#include <utility>                    // std::forward
#include "equal.hpp"                  // mozi::equality_comparer/equal
#include "struct_reflection_core.hpp" // mozi::zip
#include "type_traits.hpp"            // mozi::remove_cvref

namespace mozi {

template <typename T, typename U>
struct equality_comparer<T, U,
                         std::enable_if_t<is_reflected_struct_v<T> &&
                                          is_reflected_struct_v<U>>> {
    template <typename T1, typename U1>
    constexpr bool operator()(T1&& lhs, U1&& rhs) const
    {
        static_assert(std::is_same_v<mozi::remove_cvref_t<T1>, T> &&
                      std::is_same_v<mozi::remove_cvref_t<U1>, U>);
        if constexpr (T::_size == U::_size) {
            bool result = true;
            zip(std::forward<T1>(lhs), std::forward<U1>(rhs),
                [&result](auto /*name1*/, auto /*name2*/,
                          auto&& value1, auto&& value2) {
                    if (result) {
                        result =
                            equal(std::forward<decltype(value1)>(value1),
                                  std::forward<decltype(value2)>(value2));
                    }
                });
            return result;
        } else {
            return false;
        }
    }
};

} // namespace mozi

#define MOZI_DECLARE_EQUAL_COMPARISON(st)                                  \
    constexpr bool operator==(const st& lhs, const st& rhs)                \
    {                                                                      \
        return mozi::equal(lhs, rhs);                                      \
    }

#if !defined(DECLARE_EQUAL_COMPARISON) &&                                  \
    !defined(MOZI_NO_DECLARE_EQUAL_COMPARISON)
#define DECLARE_EQUAL_COMPARISON MOZI_DECLARE_EQUAL_COMPARISON
#endif

#endif // MOZI_STRUCT_REFLECTION_EQUAL_HPP
