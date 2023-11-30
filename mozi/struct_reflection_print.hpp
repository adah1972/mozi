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

#ifndef MOZI_STRUCT_REFLECTION_PRINT_HPP
#define MOZI_STRUCT_REFLECTION_PRINT_HPP

#include <cstddef>                    // std::size_t
#include <ostream>                    // std::ostream
#include <type_traits>                // std::enable_if
#include "print.hpp"                  // mozi::printer
#include "struct_reflection_core.hpp" // mozi::for_each
#include "type_traits.hpp"            // mozi::remove_cvref/...

namespace mozi {

namespace detail {

// Field output function for reflected structs
template <typename T>
void output_field(const T& obj, std::ostream& os, const char* name,
                  int depth)
{
    os << indent(depth) << name << ": ";
    print(obj, os, depth);
}

} // namespace detail

template <typename T>
struct printer<T, std::enable_if_t<is_reflected_struct_v<T>>> {
    void operator()(const T& obj, std::ostream& os, int depth) const
    {
        os <<  "{\n";
        for_each(obj, [size = remove_cvref_t<T>::_size, depth, &os](
                          std::size_t index, auto name, const auto& value) {
            detail::output_field(value, os, name, depth + 1);
            if (index != size - 1) {
                os << ",\n";
            } else {
                os << '\n';
            }
        });
        os << detail::indent(depth) << "}";
    }
};

} // namespace mozi

#endif // MOZI_STRUCT_REFLECTION_PRINT_HPP
