/*
 * Copyright (c) 2021 Netcan
 * Copyright (c) 2022-2023 Wu Yongwei
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

#ifndef MOZI_STRUCT_REFLECTION_CORE_HPP
#define MOZI_STRUCT_REFLECTION_CORE_HPP

#include <cstddef>                 // std::size_t
#include <cstdint>                 // SIZE_MAX
#include <type_traits>             // std::decay/enable_if/is_same/...
#include <utility>                 // std::forward/index_sequence/...
#include "metamacro.h"             // MOZI_GET_ARG_COUNT/MOZI_REPEAT_ON/...
#include "compile_time_string.hpp" // MOZI_CTS_STRING
#include "type_traits.hpp"         // mozi::is_reflected_struct

namespace mozi {

template <std::size_t N>
using index_t = std::integral_constant<std::size_t, N>;

template <typename T>
struct type_t {
    using type = T;
};

template <std::size_t I, typename T,
          std::enable_if_t<is_reflected_struct_v<std::decay_t<T>>, int> = 0>
constexpr decltype(auto) get(T&& obj)
{
    using DT = std::decay_t<T>;
    static_assert(I < DT::_size, "Index to get is out of range");
    return typename DT::template _field<T, I>(std::forward<T>(obj)).value();
}

namespace detail {

template <typename T, typename F, std::size_t... Is>
constexpr void for_each_meta_impl(F&& f, std::index_sequence<Is...>)
{
    using DT = std::decay_t<T>;
    (void(std::forward<F>(f)(
         index_t<Is>{}, DT::template _field<T, Is>::name,
         type_t<typename DT::template _field<T, Is>::type>{})),
     ...);
}

template <typename T, typename F, std::size_t... Is>
constexpr void for_each_impl(T&& obj, F&& f, std::index_sequence<Is...>)
{
    using DT = std::decay_t<T>;
    (void(std::forward<F>(f)(index_t<Is>{},
                             DT::template _field<T, Is>::name,
                             get<Is>(std::forward<T>(obj)))),
     ...);
}

template <typename T, typename U, typename F, std::size_t... Is>
constexpr void zip_impl(T&& obj1, U&& obj2, F&& f,
                        std::index_sequence<Is...>)
{
    using DT = std::decay_t<T>;
    using DU = std::decay_t<U>;
    static_assert(DT::_size == DU::_size);
    (void(std::forward<F>(f)(
         DT::template _field<T, Is>::name, DU::template _field<U, Is>::name,
         get<Is>(std::forward<T>(obj1)), get<Is>(std::forward<U>(obj2)))),
     ...);
}

} // namespace detail

template <typename T, typename F,
          std::enable_if_t<is_reflected_struct_v<T>, int> = 0>
constexpr void for_each_meta(F&& f)
{
    detail::for_each_meta_impl<T>(std::forward<F>(f),
                                  std::make_index_sequence<T::_size>{});
}

template <typename T, typename F,
          std::enable_if_t<is_reflected_struct_v<std::decay_t<T>>, int> = 0>
constexpr void for_each(T&& obj, F&& f)
{
    using DT = std::decay_t<T>;
    detail::for_each_impl(std::forward<T>(obj), std::forward<F>(f),
                          std::make_index_sequence<DT::_size>{});
}

template <typename T, typename U, typename F,
          std::enable_if_t<(is_reflected_struct_v<std::decay_t<T>> &&
                            is_reflected_struct_v<std::decay_t<U>>),
                           int> = 0>
constexpr void zip(T&& obj1, U&& obj2, F&& f)
{
    using DT = std::decay_t<T>;
    using DU = std::decay_t<U>;
    static_assert(DT::_size == DU::_size);
    detail::zip_impl(std::forward<T>(obj1), std::forward<U>(obj2),
                     std::forward<F>(f),
                     std::make_index_sequence<DT::_size>{});
}

template <typename T, typename Name,
          std::enable_if_t<is_reflected_struct_v<T>, int> = 0>
constexpr std::size_t get_index(Name /*name*/)
{
    auto result = SIZE_MAX;
    for_each_meta<T>([&result](auto index, auto name, auto /*type*/) {
        if constexpr (std::is_same_v<decltype(name), Name>) {
            result = index;
        }
        (void)index;
    });
    return result;
}

} // namespace mozi

#if __cplusplus < 202002L && defined(__GNUC__)
// The literal operator needs to be exposed in this configuration
using mozi::operator""_cts;
#endif

#define MOZI_FIELD(i, arg)                                                 \
    MOZI_PAIR(arg);                                                        \
    template <typename T>                                                  \
    struct _field<T, i> {                                                  \
        using type = decltype(std::decay_t<T>::MOZI_STRIP(arg));           \
        static constexpr auto name = MOZI_CTS_STRING(MOZI_STRIP(arg));     \
        constexpr explicit _field(T&& obj) /* NOLINT */                    \
            : obj_(std::forward<T>(obj))                                   \
        {                                                                  \
        }                                                                  \
        constexpr decltype(auto) value()                                   \
        {                                                                  \
            return (std::forward<T>(obj_).MOZI_STRIP(arg));                \
        }                                                                  \
                                                                           \
    private:                                                               \
        T&& obj_; /* NOLINT */                                             \
    };

#define MOZI_DEFINE_STRUCT(st, ...)                                        \
    struct st {                                                            \
        using is_mozi_reflected = void;                                    \
        template <typename, std::size_t>                                   \
        struct _field;                                                     \
        static constexpr std::size_t _size =                               \
            MOZI_GET_ARG_COUNT(__VA_ARGS__);                               \
        MOZI_REPEAT_ON(MOZI_FIELD, __VA_ARGS__)                            \
    }

// This macro can only be used at the global namespace, so it cannot be
// integrated into the MOZI_DEFINE_STRUCT macro, even if the functionality
// is always wanted.
#define MOZI_DECLARE_TUPLE_LIKE(st)                                        \
    template <>                                                            \
    struct std::tuple_size<st> {                                           \
        static constexpr size_t value = st::_size;                         \
    };                                                                     \
    template <size_t I>                                                    \
    struct std::tuple_element<I, st> {                                     \
        using type = typename st::_field<st, I>::type;                     \
    }

// While it is possible to provide this definition as a friend inside the
// reflected struct, it may not be necessary and can cause some compiler
// warnings.
#define MOZI_DECLARE_LESS_COMPARISON(st)                                   \
    constexpr bool operator<(const st& lhs, const st& rhs)                 \
    {                                                                      \
        static_assert(mozi::is_reflected_struct_v<st>);                    \
        int result = 0;                                                    \
        mozi::zip(lhs, rhs,                                                \
            [&result](auto /*name1*/, auto /*name2*/,                      \
                      const auto& value1, const auto& value2) {            \
                if (result == 0) {                                         \
                    if (value1 < value2) {                                 \
                        result = -1;                                       \
                    } else if (value2 < value1) {                          \
                        result = 1;                                        \
                    }                                                      \
                }                                                          \
            });                                                            \
        return result < 0;                                                 \
    }

#if !defined(DEFINE_STRUCT) && !defined(MOZI_NO_DEFINE_STRUCT)
#define DEFINE_STRUCT MOZI_DEFINE_STRUCT
#endif

#if !defined(DECLARE_TUPLE_LIKE) && !defined(MOZI_NO_DECLARE_TUPLE_LIKE)
#define DECLARE_TUPLE_LIKE MOZI_DECLARE_TUPLE_LIKE
#endif

#if !defined(DECLARE_LESS_COMPARISON) &&                                   \
    !defined(MOZI_NO_DECLARE_LESS_COMPARISON)
#define DECLARE_LESS_COMPARISON MOZI_DECLARE_LESS_COMPARISON
#endif

#endif // MOZI_STRUCT_REFLECTION_CORE_HPP
