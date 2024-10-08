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

#ifndef MOZI_BIT_FIELDS_PRINT_HPP
#define MOZI_BIT_FIELDS_PRINT_HPP

#include <ostream>                     // std::ostream
#include <type_traits>                 // std::enable_if
#include "compile_time_string.hpp"     // MOZI_CTS_GET_VALUE
#include "print.hpp"                   // mozi::printer
#include "struct_reflection_core.hpp"  // mozi::for_each
#include "struct_reflection_print.hpp" // mozi::detail::output_field
#include "type_traits.hpp"             // mozi::is_bit_fields_container

namespace mozi {

template <typename T>
struct printer<T, std::enable_if_t<is_bit_fields_container_v<T>>> {
    void operator()(const T& obj, std::ostream& os, int depth) const
    {
        os <<  "{\n";
        for_each(obj, [&os, depth](auto index, auto name,
                                   const auto& field) {
            detail::output_field(field,
                                 os, MOZI_CTS_GET_VALUE(name), depth + 1);
            if constexpr (index != remove_cvref_t<T>::_size - 1) {
                os << ",\n";
            } else {
                os << '\n';
            }
        });
        os << detail::indent(depth) << "}";
    }
};

} // namespace mozi

#endif // MOZI_BIT_FIELDS_PRINT_HPP
