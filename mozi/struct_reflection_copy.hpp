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

#ifndef MOZI_STRUCT_REFLECTION_COPY_HPP
#define MOZI_STRUCT_REFLECTION_COPY_HPP

#include <cstddef>                    // std::size_t
#include <cstdint>                    // SIZE_MAX
#include <tuple>                      // std::tuple
#include <type_traits>                // std::enable_if
#include <utility>                    // std::forward/move
#include "copy.hpp"                   // mozi::copier/copy
#include "struct_reflection_core.hpp" // mozi::zip/for_each/for_each_meta
#include "type_traits.hpp"            // mozi::is_reflected_struct

namespace mozi {

namespace detail {

template <typename T, typename U, std::size_t... Is>
constexpr void copy_tuple_like_impl(T&& src, U& dest,
                                    std::index_sequence<Is...>)
{
    using mozi::get;
    using std::get;
    (copy(get<Is>(std::forward<T>(src)), get<Is>(dest)), ...);
}

} // namespace detail

template <typename T, typename U>
struct copier<T, U,
              std::enable_if_t<is_reflected_struct_v<T> &&
                               is_reflected_struct_v<U>>> {
    void operator()(const T& src, U& dest) const
    {
        zip(src, dest,
            [](auto /*name1*/, auto /*name2*/,
               const auto& value1, auto& value2) {
                copy(value1, value2);
            });
    }
    void operator()(T&& src, U& dest) const
    {
        zip(std::move(src), dest,
            [](auto /*name1*/, auto /*name2*/,
               auto&& value1, auto& value2) {
                copy(std::forward<decltype(value1)>(value1), value2);
            });
    }
};

template <typename T, typename... Args>
struct copier<T, std::tuple<Args...>,
              std::enable_if_t<is_reflected_struct_v<T>>> {
    void operator()(const T& src, std::tuple<Args...>& dest) const
    {
        using DT = std::decay_t<T>;
        static_assert(sizeof...(Args) == DT::_size);
        detail::copy_tuple_like_impl(src, dest,
                                     std::make_index_sequence<DT::_size>{});
    }
    void operator()(T&& src, std::tuple<Args...>& dest) const
    {
        using DT = std::decay_t<T>;
        static_assert(sizeof...(Args) == DT::_size);
        detail::copy_tuple_like_impl(std::move(src), dest,
                                     std::make_index_sequence<DT::_size>{});
    }
};

template <typename T, typename... Args>
struct copier<std::tuple<Args...>, T,
              std::enable_if_t<is_reflected_struct_v<T>>> {
    void operator()(const std::tuple<Args...>& src, T& dest) const
    {
        using DT = std::decay_t<T>;
        static_assert(sizeof...(Args) == DT::_size);
        detail::copy_tuple_like_impl(src, dest,
                                     std::make_index_sequence<DT::_size>{});
    }
    void operator()(std::tuple<Args...>&& src, T& dest) const
    {
        using DT = std::decay_t<T>;
        static_assert(sizeof...(Args) == DT::_size);
        detail::copy_tuple_like_impl(std::move(src), dest,
                                     std::make_index_sequence<DT::_size>{});
    }
};

template <typename T, typename U,
          std::enable_if_t<(is_reflected_struct_v<std::decay_t<T>> &&
                            is_reflected_struct_v<std::decay_t<U>>),
                           int> = 0>
constexpr std::size_t count_missing_fields()
{
    std::size_t result = 0;
    for_each_meta<U>(
        [&result](std::size_t /*index*/, auto name, auto /*type*/) {
            if constexpr (get_index<T>(name) == SIZE_MAX) {
                ++result;
            }
        });
    return result;
}

enum class missing_fields : std::size_t {};

template <missing_fields MissingFields = missing_fields{0},
          typename T, typename U>
constexpr void copy_same_name_fields(T&& src, U& dest) // NOLINT
{
    constexpr size_t actual_missing_fields =
        count_missing_fields<std::decay_t<T>, std::decay_t<U>>();
    static_assert(size_t(MissingFields) == actual_missing_fields);
    for_each(dest, [&src](std::size_t /*index*/, auto name, auto& value) {
        using DT = std::decay_t<T>;
        constexpr auto index = get_index<DT>(name);
        if constexpr (index != SIZE_MAX) {
            copy(get<index>(std::forward<T>(src)), value);
        }
    });
}

} // namespace mozi

#endif // MOZI_STRUCT_REFLECTION_COPY_HPP
