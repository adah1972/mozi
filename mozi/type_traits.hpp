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

#ifndef MOZI_TYPE_TRAITS_HPP
#define MOZI_TYPE_TRAITS_HPP

#include <iterator>    // std::begin/end
#include <tuple>       // std::tuple_size
#include <type_traits> // std::false_type/true_type/void_t/remove_cv/...
#include <utility>     // std::pair

namespace mozi {

namespace adl {

using std::begin;
using std::end;

template <class Rng>
auto adl_begin(Rng&& rng) -> decltype(begin(rng));

template <class Rng>
auto adl_end(Rng&& rng) -> decltype(end(rng));

} // namespace adl

// Type traits for ranges
template <typename T, typename = void>
struct is_range : std::false_type {};
template <typename T>
struct is_range<T, std::void_t<decltype(adl::adl_begin(std::declval<T>()),
                                        adl::adl_end(std::declval<T>()))>>
    : std::true_type {};
template <typename T>
inline constexpr bool is_range_v = is_range<T>::value;

// Type trait to detect std::pair
template <typename T>
struct is_pair : std::false_type {};
template <typename T, typename U>
struct is_pair<std::pair<T, U>> : std::true_type {};
template <typename T>
inline constexpr bool is_pair_v = is_pair<T>::value;

// Type trait for tuple-like objects
template <typename T, typename = void>
struct is_tuple_like : std::false_type {};
template <typename T>
struct is_tuple_like<T, std::void_t<decltype(std::tuple_size<T>::value)>>
    : std::true_type {};
template <typename T>
inline constexpr bool is_tuple_like_v = is_tuple_like<T>::value;

// Type trait for char types
template <typename T, typename = void>
struct is_char : std::false_type {};
template <>
struct is_char<char> : std::true_type {};
template <>
struct is_char<signed char> : std::true_type {};
template <>
struct is_char<unsigned char> : std::true_type {};
template <typename T>
inline constexpr bool is_char_v = is_char<T>::value;

// Type trait for char pointer types
template <typename T, typename = void>
struct is_char_pointer : std::false_type {};
template <typename T>
struct is_char_pointer<
    T,
    std::enable_if_t<std::is_pointer_v<T> &&
                     is_char_v<std::remove_cv_t<std::remove_pointer_t<T>>>>>
    : std::true_type {};
template <typename T>
inline constexpr bool is_char_pointer_v = is_char_pointer<T>::value;

// Type trait to promote char types to "normal" int types
template <typename T, typename = void>
struct promote_char {
    using type = T;
};
template <typename T>
struct promote_char<
    T, std::enable_if_t<is_char_v<T> && std::is_signed_v<T>>> {
    using type = int;
};
template <typename T>
struct promote_char<
    T, std::enable_if_t<is_char_v<T> && std::is_unsigned_v<T>>> {
    using type = unsigned;
};
template <typename T>
using promote_char_t = typename promote_char<T>::type;

// C++20 remove_cvref
template <typename T>
struct remove_cvref {
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <typename T>
using remove_cvref_t = typename remove_cvref<T>::type;

// C++20-compatible underlying_type for better SFINAE than some old
// implementations
template <typename T, typename = void>
struct underlying_type {};
template <typename T>
struct underlying_type<T, std::enable_if_t<std::is_enum_v<T>>> {
    using type = std::underlying_type_t<T>;
};
template <typename T>
using underlying_type_t = typename underlying_type<T>::type;

// C++23 is_scoped_enum
template <typename T, typename = void>
struct is_scoped_enum : std::false_type {};
template <typename T>
struct is_scoped_enum<
    T, std::enable_if_t<std::is_enum_v<T> &&
                        !std::is_convertible_v<T, underlying_type_t<T>>>>
    : std::true_type {};
template <typename T>
inline constexpr bool is_scoped_enum_v = is_scoped_enum<T>::value;

// Type trait for containers
template <typename T, typename = void>
struct is_container : std::false_type {};
template <typename T>
struct is_container<T, std::void_t<typename T::value_type>>
    : std::true_type {};
template <typename T>
inline constexpr bool is_container_v = is_container<T>::value;

// Type trait for map-like containers
template <typename T, typename = void>
struct is_map : std::false_type {};
template <typename T>
struct is_map<
    T, std::void_t<decltype(std::declval<typename T::key_type&>(),
                            std::declval<typename T::mapped_type&>(),
                            std::declval<typename T::value_type&>())>>
    : std::true_type {};
template <typename T>
inline constexpr bool is_map_v = is_map<T>::value;

// Type trait for reflected enums
template <typename T, typename = void>
struct is_reflected_enum : std::false_type {};
template <typename T>
struct is_reflected_enum<T,
                         std::void_t<decltype(is_mozi_reflected_enum(T{}))>>
    : std::true_type {};
template <typename T>
inline constexpr static bool is_reflected_enum_v =
    is_reflected_enum<T>::value;

// Type trait for reflected structs
template <typename T, typename = void>
struct is_reflected_struct : std::false_type {};
template <typename T>
struct is_reflected_struct<T, std::void_t<typename T::is_mozi_reflected>>
    : std::true_type {};
template <typename T>
inline constexpr static bool is_reflected_struct_v =
    is_reflected_struct<T>::value;

} // namespace mozi

#endif // MOZI_TYPE_TRAITS_HPP
