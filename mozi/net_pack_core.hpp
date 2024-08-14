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

#ifndef MOZI_NET_PACK_CORE_HPP
#define MOZI_NET_PACK_CORE_HPP

#include <climits>           // CHAR_BIT
#include "serialization.hpp" // mozi::serialize/deserialize/...

namespace mozi::net_pack {

template <typename T, typename = void>
struct serializer;

namespace detail {

struct serialize_fn {
    static_assert(CHAR_BIT == 8);

    template <typename T>
    void operator()(const T& value, serialize_t& dest) const
    {
        mozi::serialize(value, dest, serializer_list<serializer>{});
    }

    template <typename T>
    serialize_t operator()(const T& value) const
    {
        serialize_t result;
        operator()(value, result);
        return result;
    }
};

struct deserialize_fn {
    template <typename T>
    deserialize_result operator()(T& value, deserialize_t& src) const
    {
        return mozi::deserialize(value, src, serializer_list<serializer>{});
    }
};

} // namespace detail

inline constexpr detail::serialize_fn serialize{};
inline constexpr detail::deserialize_fn deserialize{};

} // namespace mozi::net_pack

#endif // MOZI_NET_PACK_CORE_HPP
