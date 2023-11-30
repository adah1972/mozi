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
#include <tuple>           // std::tuple_element
#include <type_traits>     // std::enable_if/decay/is_same
#include <utility>         // std::forward/pair/index_sequence/...
#include "type_traits.hpp" // miscellaneous type traits

namespace mozi {

template <typename T, typename = void>
struct printer {
    void operator()(const T& obj, std::ostream& os, int /*depth*/) const
    {
        os << obj;
    }
};

template <typename T>
void print(T&& obj, std::ostream& os = std::cout, int depth = 0)
{
    printer<remove_cvref_t<T>>{}(std::forward<T>(obj), os, depth);
}

template <typename T>
void println(T&& obj, std::ostream& os = std::cout, int depth = 0)
{
    print(std::forward<T>(obj), os, depth);
    os << '\n';
}

namespace detail {

// Stream manipulator for indentation
class indent {
public:
    explicit indent(int n) : n_(n) {}
    template <typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>&
    operator<<(std::basic_ostream<CharT, Traits>& os, const indent& x)
    {
        for (int i = 0; i < x.n_; ++i) {
            os << "    ";
        }
        return os;
    }

private:
    int n_;
};

// Helper functions to check whether indentation and newline need to be
// inserted between objects of specified type
template <typename T>
constexpr bool needs_indent();
template <typename T, std::size_t... Is>
constexpr bool needs_indent_tuple(std::index_sequence<Is...>)
{
    return (needs_indent<std::tuple_element_t<Is, T>>() || ...);
}
template <typename T>
constexpr bool needs_indent()
{
    if constexpr (is_reflected_struct_v<T>) {
        return true;
    } else if constexpr (is_container_v<T>) {
        return needs_indent<typename T::value_type>();
    } else if constexpr (is_tuple_like_v<T>) {
        return needs_indent_tuple<T>(
            std::make_index_sequence<std::tuple_size_v<T>>{});
    }
    return false;
}

// Element output function for containers that define a key_type and
// have its value type as std::pair
template <typename T>
void output_element(std::ostream& os, const T& element, int depth,
                    std::true_type)
{
    print(element.first, os, depth);
    os << " => ";
    print(element.second, os, depth);
}

// Element output function for other ranges
template <typename T>
void output_element(std::ostream& os, const T& element, int depth,
                    std::false_type)
{
    print(element, os, depth);
}

// Member output function for tuple-like objects
template <typename Tup, std::size_t... Is>
void output_tuple_members(std::ostream& os, const Tup& tup, int depth,
                          std::index_sequence<Is...>)
{
    using std::get;
    ((os << (Is != 0 ? ", " : ""), print(get<Is>(tup), os, depth)), ...);
}

} // namespace detail

template <typename T>
struct printer<T, std::enable_if_t<std::is_same_v<T, char> ||
                                   std::is_same_v<T, signed char>>> {
    void operator()(T ch, std::ostream& os, int /*depth*/) const
    {
        // chars and signed chars are output as characters
        os << '\'' << ch << '\'';
    }
};

template <typename T>
struct printer<T, std::enable_if_t<std::is_same_v<T, unsigned char> ||
                                   std::is_same_v<T, std::byte>>> {
    void operator()(T value, std::ostream& os, int /*depth*/) const
    {
        // unsigned chars and bytes are output as unsigned integers
        os << static_cast<unsigned>(value);
    }
};

template <typename T>
struct printer<T, std::enable_if_t<is_char_pointer_v<T>>> {
    void operator()(T ptr, std::ostream& os, int /*depth*/) const
    {
        // char* etc. are output as strings
        os << '"' << ptr << '"';
    }
};

template <typename T>
struct printer<T, std::enable_if_t<is_range_v<T>>> {
    template <typename U>
    // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward)
    void operator()(U&& rng, std::ostream& os, int depth) const
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
            constexpr bool needs_indent = detail::needs_indent<element_type>();
            constexpr char sep = needs_indent ? '\n' : ' ';

            os << '{';
            auto last = end(rng);
            bool on_first_element = true;
            for (auto it = begin(rng); it != last; ++it) {
                if (on_first_element) {
                    on_first_element = false;
                } else {
                    os << ',';
                }
                os << sep;
                if constexpr (needs_indent) {
                    os << detail::indent(depth + 1);
                }
                detail::output_element(
                    os, *it, depth + (needs_indent ? 1 : 0), is_map<T>{});
            }
            if (!on_first_element) { // range is not empty
                os << sep;
                if constexpr (needs_indent) {
                    os << detail::indent(depth);
                }
            }
            os << '}';
        }
    }
};

template <typename T>
struct printer<T, std::enable_if_t<is_tuple_like_v<T> && !is_range_v<T> &&
                                   !is_reflected_struct_v<T>>> {
    void operator()(const T& tup, std::ostream& os, int depth) const
    {
        os << '(';
        detail::output_tuple_members(
            os, tup, depth,
            std::make_index_sequence<std::tuple_size_v<T>>{});
        os << ')';
    }
};

} // namespace mozi

#endif // MOZI_PRINT_HPP
