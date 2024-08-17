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

#ifndef MOZI_BIT_FIELDS_CORE_HPP
#define MOZI_BIT_FIELDS_CORE_HPP

#include <climits>                    // UINT32_MAX
#include <cstddef>                    // std::size_t
#include <cstdint>                    // std::uint8_t/uint16_t/uint32_t
#include <type_traits>                // std::enable_if
#include "metamacro.h"                // MOZI_GET_ARG_COUNT/MOZI_REPEAT_ON
#include "struct_reflection_core.hpp" // mozi::for_each_meta/MOZI_FIELD
#include "type_traits.hpp"            // mozi::is_bit_fields_container/...

namespace mozi {

namespace detail {

template <std::size_t N, typename = void>
struct bits_storage;

template <std::size_t N>
struct bits_storage<N, std::enable_if_t<(N > 0 && N <= 8)>> {
    using type = std::uint8_t;
};

template <std::size_t N>
struct bits_storage<N, std::enable_if_t<(N > 8 && N <= 16)>> {
    using type = std::uint16_t;
};

template <std::size_t N>
struct bits_storage<N, std::enable_if_t<(N > 16 && N <= 32)>> {
    using type = std::uint32_t;
};

constexpr std::uint32_t get_bit_field_mask(unsigned len)
{
    // Shift count overflow is undefined behavior
    if (len >= 32) {
        return UINT32_MAX;
    }
    return ~(UINT32_MAX << len);
}

} // namespace detail

template <std::size_t N>
struct bit_field {
public:
    static_assert(sizeof(unsigned) >= sizeof(std::uint32_t));

    using value_type = typename detail::bits_storage<N>::type;
    static constexpr std::size_t length = N;
    value_type value;

    constexpr bit_field& operator=(unsigned n)
    {
        value = n;
        return *this;
    }
    constexpr operator unsigned() const
    {
        return value;
    }
};

namespace detail {

template <typename T>
struct is_bit_field : std::false_type {};
template <std::size_t N>
struct is_bit_field<bit_field<N>> : std::true_type {};
template <typename T>
inline constexpr bool is_bit_field_v = is_bit_field<T>::value;

} // namespace detail

template <typename T,
          std::enable_if_t<mozi::is_bit_fields_container_v<T>, int> = 0>
constexpr std::size_t count_bit_fields()
{
    std::size_t result{};
    mozi::for_each_meta<T>([&](auto /*index*/, auto /*name*/, auto type) {
        using field_type = typename decltype(type)::type;
        static_assert(detail::is_bit_field_v<field_type>);
        result += field_type::length;
    });
    return result;
}

} // namespace mozi

#define MOZI_DEFINE_BIT_FIELDS_CONTAINER(st, ...)                          \
    struct st {                                                            \
        using is_mozi_reflected = void;                                    \
        using is_mozi_bit_fields_container = void;                         \
        template <typename, std::size_t>                                   \
        struct _field;                                                     \
        static constexpr std::size_t _size =                               \
            MOZI_GET_ARG_COUNT(__VA_ARGS__);                               \
        MOZI_REPEAT_ON(MOZI_FIELD, __VA_ARGS__)                            \
    }

#if !defined(DEFINE_BIT_FIELDS_CONTAINER) &&                               \
    !defined(MOZI_NO_DEFINE_BIT_FIELDS_CONTAINER)
#define DEFINE_BIT_FIELDS_CONTAINER MOZI_DEFINE_BIT_FIELDS_CONTAINER
#endif

#endif // MOZI_BIT_FIELDS_CORE_HPP
