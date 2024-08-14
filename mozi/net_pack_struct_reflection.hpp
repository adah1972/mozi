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

#ifndef MOZI_NET_PACK_STRUCT_REFLECTION_HPP
#define MOZI_NET_PACK_STRUCT_REFLECTION_HPP

#include <type_traits>                // std::enable_if
#include "net_pack_core.hpp"          // mozi::net_pack::serializer
#include "serialization.hpp"          // mozi::serialize/deserialize/...
#include "struct_reflection_core.hpp" // mozi::for_each
#include "type_traits.hpp"            // mozi::is_reflected_struct

namespace mozi::net_pack {

template <typename T>
struct serializer<T, std::enable_if_t<mozi::is_reflected_struct_v<T>>> {
    template <typename SerializerList>
    static void serialize(T obj, serialize_t& dest,
                          SerializerList serializers)
    {
        mozi::for_each(
            obj, [&](auto /*index*/, auto /*name*/, const auto& value) {
                mozi::serialize(value, dest, serializers);
            });
    }

    template <typename SerializerList>
    static deserialize_result deserialize(T& obj, deserialize_t& src,
                                          SerializerList serializers)
    {
        auto result = deserialize_result::success;
        mozi::for_each(
            obj, [&](auto /*index*/, auto /*name*/, auto& value) {
                if (result == deserialize_result::success) {
                    result = mozi::deserialize(value, src, serializers);
                }
            });
        return result;
    }
};

} // namespace mozi::net_pack

#endif // MOZI_NET_PACK_STRUCT_REFLECTION_HPP
