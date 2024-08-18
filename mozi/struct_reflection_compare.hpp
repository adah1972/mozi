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
#include "struct_reflection_core.hpp"  // IWYU pragma: keep
#include "struct_reflection_equal.hpp" // IWYU pragma: keep

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)

#include <compare>                     // IWYU pragma: keep

#define MOZI_DECLARE_COMPARISON_IMP(st)                                    \
    MOZI_DECLARE_EQUAL_COMPARISON(st)                                      \
    constexpr std::strong_ordering operator<=>(const st& lhs,              \
                                               const st& rhs)              \
    {                                                                      \
        static_assert(mozi::is_reflected_struct_v<st>);                    \
        std::strong_ordering result = std::strong_ordering::equal;         \
        mozi::zip(lhs, rhs,                                                \
                  [&result](auto /*name1*/, auto /*name2*/,                \
                            const auto& value1, const auto& value2) {      \
                      if (result == std::strong_ordering::equal) {         \
                          if (mozi::equal(value1, value2)) {               \
                              return;                                      \
                          }                                                \
                          if (value1 < value2) {                           \
                              result = std::strong_ordering::less;         \
                          } else {                                         \
                              result = std::strong_ordering::greater;      \
                          }                                                \
                      }                                                    \
                  });                                                      \
        return result;                                                     \
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
