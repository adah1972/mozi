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

#ifndef MOZI_NET_PACK_BIT_FIELDS_HPP
#define MOZI_NET_PACK_BIT_FIELDS_HPP

#include <climits>                    // CHAR_BIT/UINT32_MAX
#include <cstddef>                    // std::size_t
#include <cstdint>                    // std::uint32_t
#include <type_traits>                // std::enable_if
#include "bit_fields_core.hpp"        // mozi::count_bit_fields/...
#include "net_pack_core.hpp"          // mozi::net_pack::serializer
#include "serialization.hpp"          // mozi::serialize/deserialize/...
#include "struct_reflection_core.hpp" // mozi::for_each
#include "type_traits.hpp"            // mozi::is_bit_fields_container/...

namespace mozi::net_pack {

namespace detail {

constexpr std::uint32_t get_bit_field_mask(unsigned len)
{
    return ~(UINT32_MAX << len);
}

} // namespace detail

template <typename T>
struct serializer<T, std::enable_if_t<mozi::is_bit_fields_container_v<T>>> {
    static constexpr std::size_t size_bits = count_bit_fields<T>();
    static_assert(size_bits == 8 || size_bits == 16 || size_bits == 32,
                  "A bit-fields container must have 8, 16, or 32 bits");

    template <typename SerializerList>
    static void serialize(T obj, serialize_t& dest,
                          SerializerList serializers)
    {
        using value_type =
            typename mozi::detail::bits_storage<size_bits>::type;
        value_type value{};
        mozi::for_each(
            obj, [&](auto /*index*/, auto /*name*/, const auto& field) {
                value <<= remove_cvref_t<decltype(field)>::length;
                value |= field.value;
            });
        mozi::serialize(value, dest, serializers);
    }

    template <typename SerializerList>
    static deserialize_result deserialize(T& obj, deserialize_t& src,
                                          SerializerList serializers)
    {
        using value_type =
            typename mozi::detail::bits_storage<size_bits>::type;
        constexpr unsigned total_len = sizeof(value_type) * CHAR_BIT;
        value_type value{};
        auto ec = mozi::deserialize(value, src, serializers);
        if (ec == deserialize_result::success) {
            mozi::for_each(
                obj, [&](auto /*index*/, auto /*name*/, auto& field) {
                    constexpr unsigned len =
                        remove_cvref_t<decltype(field)>::length;
                    field.value = ((unsigned{value} >> (total_len - len)) &
                                   detail::get_bit_field_mask(len));
                    value <<= len;
                });
        }
        return ec;
    }
};

} // namespace mozi::net_pack

#endif // MOZI_NET_PACK_BIT_FIELDS_HPP
