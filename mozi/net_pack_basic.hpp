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

#ifndef MOZI_NET_PACK_BASIC_HPP
#define MOZI_NET_PACK_BASIC_HPP

#include <array>             // std::array
#include <climits>           // CHAR_BIT/UCHAR_MAX
#include <cstddef>           // std::byte/size_t
#include <type_traits>       // std::enable_if/is_integral/is_enum
#include "net_pack_core.hpp" // mozi::net_pack::serializer
#include "serialization.hpp" // mozi::deserialize_result/...
#include "type_traits.hpp"   // mozi::is_char/underlying_type

namespace mozi::net_pack {

template <>
struct serializer<bool> {
    static void serialize(bool value, serialize_t& dest)
    {
        dest.push_back(std::byte{value});
    }

    static deserialize_result deserialize(bool& value, deserialize_t& src)
    {
        if (src.empty()) {
            return deserialize_result::input_truncated;
        }
        if (src.front() == std::byte{0}) {
            value = false;
            src = src.subspan(1);
            return deserialize_result::success;
        }
        if (src.front() == std::byte{1}) {
            value = true;
            src = src.subspan(1);
            return deserialize_result::success;
        }
        return deserialize_result::invalid_value;
    }

    template <typename SerializerList>
    static void serialize(bool value, serialize_t& dest,
                          SerializerList /*unused*/)
    {
        serialize(value, dest);
    }
    template <typename SerializerList>
    static deserialize_result deserialize(bool& value, deserialize_t& src,
                                          SerializerList /*unused*/)
    {
        return deserialize(value, src);
    }
};

template <typename T>
struct serializer<T, std::enable_if_t<is_char_v<T>>> {
    static void serialize(T value, serialize_t& dest)
    {
        dest.push_back(static_cast<std::byte>(value));
    }

    static deserialize_result deserialize(T& value, deserialize_t& src)
    {
        if (src.empty()) {
            return deserialize_result::input_truncated;
        }
        value = static_cast<T>(src.front());
        src = src.subspan(1);
        return deserialize_result::success;
    }

    template <typename SerializerList>
    static void serialize(T value, serialize_t& dest,
                          SerializerList /*unused*/)
    {
        serialize(value, dest);
    }
    template <typename SerializerList>
    static deserialize_result deserialize(T& value, deserialize_t& src,
                                          SerializerList /*unused*/)
    {
        return deserialize(value, src);
    }
};

namespace detail {

// A nice optimizing compiler can reduce this function to a single
// instruction, if it is available on the platform (like bswap on x86).
template <typename T>
constexpr std::array<std::byte, sizeof(T)> net_convert(T value)
{
    std::array<std::byte, sizeof(T)> result{};
    constexpr auto mask = static_cast<std::make_unsigned_t<T>>(UCHAR_MAX);
    constexpr auto len = sizeof(T);
    auto unsigned_value = static_cast<std::make_unsigned_t<T>>(value);
    for (std::size_t i = 0; i < len; ++i) {
        result[len - i - 1] = static_cast<std::byte>(unsigned_value & mask);
        unsigned_value >>= CHAR_BIT;
    }
    return result;
}

} // namespace detail

template <typename T>
struct serializer<
    T, std::enable_if_t<std::is_integral_v<T> && (sizeof(T) > 1)>> {
    static void serialize(T value, serialize_t& dest)
    {
        auto net_value = detail::net_convert(value);
        dest.insert(dest.end(), net_value.begin(), net_value.end());
    }

    static deserialize_result deserialize(T& value, deserialize_t& src)
    {
        if (src.size() < sizeof(T)) {
            return deserialize_result::input_truncated;
        }
        std::make_unsigned_t<T> unsigned_value{};
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            unsigned_value <<= CHAR_BIT;
            unsigned_value |= static_cast<unsigned char>(src[i]);
        }
        value = static_cast<T>(unsigned_value);
        src = src.subspan(sizeof(T));
        return deserialize_result::success;
    }

    template <typename SerializerList>
    static void serialize(T value, serialize_t& dest,
                          SerializerList /*unused*/)
    {
        serialize(value, dest);
    }
    template <typename SerializerList>
    static deserialize_result deserialize(T& value, deserialize_t& src,
                                          SerializerList /*unused*/)
    {
        return deserialize(value, src);
    }
};

template <typename T>
struct serializer<T, std::enable_if_t<std::is_enum_v<T>>> {
    template <typename SerializerList>
    static void serialize(T value, serialize_t& dest, SerializerList serializers)
    {
        mozi::serialize(static_cast<mozi::underlying_type_t<T>>(value),
                        dest, serializers);
    }

    template <typename SerializerList>
    static deserialize_result deserialize(T& value, deserialize_t& src,
                                          SerializerList serializers)
    {
        mozi::underlying_type_t<T> temp;
        auto result = mozi::deserialize(temp, src, serializers);
        if (result == deserialize_result::success) {
            value = static_cast<T>(temp);
        }
        return result;
    }
};

} // namespace mozi::net_pack

#endif // MOZI_NET_PACK_BASIC_HPP
