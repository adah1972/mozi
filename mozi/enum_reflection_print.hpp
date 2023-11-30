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

#ifndef MOZI_ENUM_REFLECTION_PRINT_HPP
#define MOZI_ENUM_REFLECTION_PRINT_HPP

#include <ostream>                  // std::ostream
#include <type_traits>              // std::enable_if
#include "enum_reflection_core.hpp" // IWYU pragma: keep mozi::to_string
#include "print.hpp"                // mozi::printer
#include "type_traits.hpp"          // mozi::is_reflected_enum

namespace mozi {

template <typename T>
struct printer<
    T, std::enable_if_t<is_reflected_enum_v<T> && !is_scoped_enum_v<T>>> {
    void operator()(T value, std::ostream& os, int /*depth*/) const
    {
        os << to_string(value);
    }
};

template <typename T>
struct printer<
    T, std::enable_if_t<is_reflected_enum_v<T> && is_scoped_enum_v<T>>> {
    void operator()(T value, std::ostream& os, int /*depth*/) const
    {
        os << to_string(value, mozi::enum_to_string::show_name);
    }
};

} // namespace mozi

#endif // MOZI_ENUM_REFLECTION_PRINT_HPP
