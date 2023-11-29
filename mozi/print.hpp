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

#ifndef MOZI_PRINT_HPP
#define MOZI_PRINT_HPP

#include <cstddef>         // std::size_t/byte
#include <iostream>        // std::cout
#include <ostream>         // std::ostream
#include <string>          // std::string
#include <string_view>     // std::string_view
#include <type_traits>     // std::enable_if/decay/is_same
#include <utility>         // std::forward/pair/index_sequence/...
#include "type_traits.hpp" // miscellaneous type traits

namespace mozi {

template <typename T, typename = void>
struct printer {
    void operator()(const T& obj, std::ostream& os) const
    {
        os << obj;
    }
};

template <typename T>
void print(T&& obj, std::ostream& os = std::cout)
{
    printer<remove_cvref_t<T>>{}(std::forward<T>(obj), os);
}

template <typename T>
void println(T&& obj, std::ostream& os = std::cout)
{
    print(std::forward<T>(obj), os);
    os << '\n';
}

namespace detail {

// Element output function for containers that define a key_type and
// have its value type as std::pair
template <typename T, typename Rng>
auto output_element(std::ostream& os, const T& element, const Rng&,
                    std::true_type)
    -> decltype(std::declval<typename Rng::key_type>(), void())
{
    print(element.first, os);
    os << " => ";
    print(element.second, os);
}

// Element output function for other ranges
template <typename T, typename Rng>
auto output_element(std::ostream& os, const T& element, const Rng&, ...)
    -> void
{
    print(element, os);
}

// Member output function for tuple-like objects
template <typename Tup, std::size_t... Is>
void output_tuple_members(std::ostream& os, const Tup& tup,
                          std::index_sequence<Is...>)
{
    using std::get;
    ((os << (Is != 0 ? ", " : ""), print(get<Is>(tup), os)), ...);
}

} // namespace detail

template <typename T>
struct printer<T, std::enable_if_t<std::is_same_v<T, char> ||
                                   std::is_same_v<T, signed char>>> {
    void operator()(T ch, std::ostream& os) const
    {
        // chars and signed chars are output as characters
        os << '\'' << ch << '\'';
    }
};

template <typename T>
struct printer<T, std::enable_if_t<std::is_same_v<T, unsigned char> ||
                                   std::is_same_v<T, std::byte>>> {
    void operator()(T value, std::ostream& os) const
    {
        // unsigned chars and bytes are output as unsigned integers
        os << static_cast<unsigned>(value);
    }
};

template <typename T>
struct printer<T, std::enable_if_t<is_char_pointer_v<T>>> {
    void operator()(T ptr, std::ostream& os) const
    {
        // char* etc. are output as strings
        os << '"' << ptr << '"';
    }
};

template <typename T>
struct printer<T, std::enable_if_t<is_range_v<T>>> {
    template <typename U>
    // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
    void operator()(U&& rng, std::ostream& os) const
    {
        static_assert(std::is_same_v<T, remove_cvref_t<U>>);

        if constexpr (std::is_same_v<T, std::string> ||
                      std::is_same_v<T, std::string_view> ||
                      is_char_pointer_v<std::decay_t<T>>) {
            // string, string_view, and string literals
            os << '"' << rng << '"';
        } else {
            using std::begin;
            using std::end;
            using element_type = remove_cvref_t<decltype(*begin(rng))>;

            os << '{';
            auto last = end(rng);
            bool on_first_element = true;
            for (auto it = begin(rng); it != last; ++it) {
                if (!on_first_element) {
                    os << ", ";
                } else {
                    os << ' ';
                    on_first_element = false;
                }
                detail::output_element(os, *it, rng,
                                       is_pair<element_type>{});
            }
            if (!on_first_element) { // Not empty
                os << ' ';
            }
            os << '}';
        }
    }
};

template <typename T>
struct printer<T, std::enable_if_t<is_tuple_like_v<T> && !is_range_v<T>>> {
    void operator()(const T& tup, std::ostream& os) const
    {
        os << '(';
        detail::output_tuple_members(
            os, tup, std::make_index_sequence<std::tuple_size_v<T>>{});
        os << ')';
    }
};

} // namespace mozi

#endif // MOZI_PRINT_HPP
