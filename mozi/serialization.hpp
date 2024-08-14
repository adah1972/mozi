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

#ifndef MOZI_SERIALIZE_HPP
#define MOZI_SERIALIZE_HPP

#include <cstddef>         // std::byte
#include <type_traits>     // std::is_same
#include <vector>          // std::vector
#include "span.hpp"        // mozi::span
#include "type_traits.hpp" // mozi::always_false/is_type_complete

namespace mozi {

using serialize_t = std::vector<std::byte>;
using deserialize_t = span<const std::byte>;

enum class deserialize_result {
    success,
    input_truncated,
    invalid_value,
};

// Type for storing a list of serializers.  There are no data members and
// only the type counts.
//
// A serializer shall have two template parameters: the first is the type to
// be serialized, and the second is used for SFINAE.  A serializer should
// have two static member functions, specialized for supported types.  See
// net_pack_basic.hpp for examples.
template <template <typename, typename> class... Serializers>
struct serializer_list;
template <template <typename, typename> class FirstSerializer,
          template <typename, typename> class... OtherSerializers>
struct serializer_list<FirstSerializer, OtherSerializers...> {
    template <typename T>
    using first_serializer = FirstSerializer<T, void>;
    using other_serializers = serializer_list<OtherSerializers...>;
};

namespace detail {

struct serialize_fn {
    template <typename T,
              typename SerializerListCurr,
              typename SerializerListFull>
    void try_serialize(const T& value, serialize_t& dest,
                       SerializerListCurr /*current*/,
                       SerializerListFull all_serializers) const
    {
        using type = std::remove_cv_t<T>;
        if constexpr (is_type_complete_v<
                          typename SerializerListCurr::
                              template first_serializer<type>>) {
            SerializerListCurr::template first_serializer<type>::serialize(
                value, dest, all_serializers);
        } else if constexpr (std::is_same_v<
                                 typename SerializerListCurr::other_serializers,
                                 serializer_list<>>) {
            static_assert(always_false_v<T>,
                          "Serialization for type is not defined");
        } else {
            try_serialize(value, dest,
                          typename SerializerListCurr::other_serializers{},
                          all_serializers);
        }
    }
    template <typename T, typename SerializerList>
    void operator()(const T& value, serialize_t& dest,
                    SerializerList serializers) const
    {
        try_serialize(value, dest, serializers, serializers);
    }
};

struct deserialize_fn {
    template <typename T,
              typename SerializerListCurr,
              typename SerializerListFull>
    deserialize_result
    try_deserialize(T& value, deserialize_t& src,
                    SerializerListCurr /*current*/,
                    SerializerListFull all_serializers) const
    {
        using type = std::remove_cv_t<T>;
        if constexpr (is_type_complete_v<
                          typename SerializerListCurr::
                              template first_serializer<type>>) {
            return SerializerListCurr::template first_serializer<
                type>::deserialize(value, src, all_serializers);
        } else if constexpr (std::is_same_v<typename SerializerListCurr::
                                                other_serializers,
                                            serializer_list<>>) {
            static_assert(always_false_v<T>,
                          "Serialization for type is not defined");
        } else {
            return try_deserialize(
                value, src,
                typename SerializerListCurr::other_serializers{},
                all_serializers);
        }
    }
    template <typename T, typename SerializerList>
    deserialize_result operator()(T& value, deserialize_t& src,
                                  SerializerList serializers) const
    {
        return try_deserialize(value, src, serializers, serializers);
    }
};

} // namespace detail

inline constexpr detail::serialize_fn serialize{};
inline constexpr detail::deserialize_fn deserialize{};

} // namespace mozi

#endif // MOZI_SERIALIZE_HPP
