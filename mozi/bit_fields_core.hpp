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

#include <cstddef>                    // std::size_t
#include <cstdint>                    // std::uint8_t/uint16_t/uint32_t/...
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

enum class bit_field_signedness { is_unsigned, is_signed };

inline constexpr auto bit_field_unsigned = bit_field_signedness::is_unsigned;
inline constexpr auto bit_field_signed = bit_field_signedness::is_signed;

template <std::size_t N,
          bit_field_signedness Signedness = bit_field_unsigned>
class bit_field {
public:
    static_assert(sizeof(unsigned) >= sizeof(std::uint32_t));

    using value_type = typename detail::bits_storage<N>::type;
    static constexpr std::size_t length = N;
    static constexpr auto signedness = Signedness;

    constexpr bit_field() = default;

    template <bit_field_signedness S = Signedness,
              std::enable_if_t<S == bit_field_unsigned, int> = 0>
    constexpr bit_field(unsigned value)
    {
        *this = value;
    }
    template <bit_field_signedness S = Signedness,
              std::enable_if_t<S == bit_field_signed, int> = 0>
    constexpr bit_field(int value)
    {
        *this = value;
    }

    template <bit_field_signedness S = Signedness,
              std::enable_if_t<S == bit_field_unsigned, int> = 0>
    constexpr bit_field& operator=(unsigned value)
    {
        value_ = value & detail::get_bit_field_mask(N);
        return *this;
    }
    template <bit_field_signedness S = Signedness,
              std::enable_if_t<S == bit_field_signed, int> = 0>
    constexpr bit_field& operator=(int value)
    {
        value_ =
            static_cast<value_type>(value) & detail::get_bit_field_mask(N);
        return *this;
    }

    constexpr value_type underlying_value() const
    {
        return value_;
    }

    template <bit_field_signedness S = Signedness,
              std::enable_if_t<S == bit_field_unsigned, int> = 0>
    constexpr operator unsigned() const
    {
        return value_;
    }
    template <bit_field_signedness S = Signedness,
              std::enable_if_t<S == bit_field_signed, int> = 0>
    constexpr operator int() const
    {
        // Sign extension is needed in this case, and two's complement is
        // assumed.
        static_assert(N > 1);
        constexpr unsigned sign_bit = 1 << (N - 1);
        bool is_positive = (unsigned{value_} & sign_bit) == 0;
        if (is_positive) {
            return static_cast<int>(value_);
        }
        return static_cast<int>(unsigned{value_} |
                                ~detail::get_bit_field_mask(N));
    }

private:
    value_type value_;
};

template <std::size_t N, bit_field_signedness S>
constexpr bool operator==(const bit_field<N, S>& lhs,
                          const bit_field<N, S>& rhs)
{
    return lhs.underlying_value() == rhs.underlying_value();
}

#if __cplusplus >= 202002L || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L)

template <std::size_t N, bit_field_signedness S>
constexpr auto operator<=>(const bit_field<N, S>& lhs,
                           const bit_field<N, S>& rhs)
{
    if constexpr (S == bit_field_unsigned) {
        return static_cast<unsigned>(lhs) <=> static_cast<unsigned>(rhs);
    } else {
        return static_cast<int>(lhs) <=> static_cast<int>(rhs);
    }
}

#else

template <std::size_t N, bit_field_signedness S>
constexpr bool operator!=(const bit_field<N, S>& lhs,
                          const bit_field<N, S>& rhs)
{
    return !(lhs == rhs);
}

template <std::size_t N, bit_field_signedness S>
constexpr bool operator<(const bit_field<N, S>& lhs,
                         const bit_field<N, S>& rhs)
{
    if constexpr (S == bit_field_unsigned) {
        return static_cast<unsigned>(lhs) < static_cast<unsigned>(rhs);
    } else {
        return static_cast<int>(lhs) < static_cast<int>(rhs);
    }
}

template <std::size_t N, bit_field_signedness S>
constexpr bool operator>(const bit_field<N, S>& lhs,
                         const bit_field<N, S>& rhs)
{
    return rhs < lhs;
}

template <std::size_t N, bit_field_signedness S>
constexpr bool operator<=(const bit_field<N, S>& lhs,
                          const bit_field<N, S>& rhs)
{
    return !(rhs < lhs);
}

template <std::size_t N, bit_field_signedness S>
constexpr bool operator>=(const bit_field<N, S>& lhs,
                          const bit_field<N, S>& rhs)
{
    return !(lhs < rhs);
}

#endif

namespace detail {

template <typename T>
struct is_bit_field : std::false_type {};
template <std::size_t N, bit_field_signedness Signedness>
struct is_bit_field<bit_field<N, Signedness>> : std::true_type {};
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
