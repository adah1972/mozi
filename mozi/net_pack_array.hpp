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

#ifndef MOZI_NET_PACK_ARRAY_HPP
#define MOZI_NET_PACK_ARRAY_HPP

#include <array>             // std::array
#include <cstddef>           // std::size_t
#include "net_pack_core.hpp" // mozi::net_pack::serializer
#include "serialization.hpp" // mozi::serialize/deserialize/...

namespace mozi::net_pack {

template <typename T, std::size_t N>
struct serializer<T[N]> {
    template <typename SerializerList>
    static void serialize(const T (&arr)[N], serialize_t& dest,
                          SerializerList serializers)
    {
        for (const auto& value : arr) {
            mozi::serialize(value, dest, serializers);
        }
    }

    template <typename SerializerList>
    static deserialize_result deserialize(T (&arr)[N], deserialize_t& src,
                                          SerializerList serializers)
    {
        for (auto& value : arr) {
            auto result = mozi::deserialize(value, src, serializers);
            if (result != deserialize_result::success) {
                return result;
            }
        }
        return deserialize_result::success;
    }
};

template <typename T, std::size_t N>
struct serializer<std::array<T, N>> {
    template <typename SerializerList>
    static void serialize(const std::array<T, N>& arr, serialize_t& dest,
                          SerializerList serializers)
    {
        for (const auto& value : arr) {
            mozi::serialize(value, dest, serializers);
        }
    }

    template <typename SerializerList>
    static deserialize_result deserialize(std::array<T, N>& arr,
                                          deserialize_t& src,
                                          SerializerList serializers)
    {
        for (auto& value : arr) {
            auto result = mozi::deserialize(value, src, serializers);
            if (result != deserialize_result::success) {
                return result;
            }
        }
        return deserialize_result::success;
    }
};

} // namespace mozi::net_pack

#endif // MOZI_NET_PACK_BASIC_HPP
