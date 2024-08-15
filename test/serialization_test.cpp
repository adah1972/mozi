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

#include "mozi/serialization.hpp"       // mozi::serialize/deserialize/...
#include <array>                        // std::array/begin/end
#include <cstddef>                      // std::size_t/byte
#include <cstdint>                      // std::uint8_t/uint16_t/uint32_t
#include <cstring>                      // std::memcpy
#include <stdexcept>                    // std::logic_error/runtime_error/...
#include <tuple>                        // std::tuple
#include <type_traits>                  // std::is_standard_layout/...
#include <catch2/catch_test_macros.hpp> // Catch2 test macros
#include "mozi/equal.hpp"               // mozi::equal
#include "mozi/net_pack.hpp"            // mozi::net_pack::serializer/...
#include "mozi/span.hpp"                // mozi::span
#include "mozi/struct_reflection.hpp"   // DEFINE_STRUCT

#if MOZI_SERIALIZATION_USES_PMR == 1
#include <memory_resource>              // std::pmr::memory_resource
#endif

using mozi::deserialize_result;

namespace {

template <typename T, std::size_t N>
auto make_byte_span(const T (&a)[N])
{
    return mozi::span(
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<const std::byte*>(std::begin(a)),
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        reinterpret_cast<const std::byte*>(std::end(a)));
}

#if MOZI_SERIALIZATION_USES_PMR == 1
class static_once_buffer : public std::pmr::memory_resource {
public:
    static_once_buffer(void* ptr, std::size_t len) : ptr_(ptr), len_(len) {}
    static_once_buffer(const static_once_buffer&) = delete;
    static_once_buffer& operator=(const static_once_buffer&) = delete;
    ~static_once_buffer() override = default;

private:
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    void* do_allocate(std::size_t bytes, std::size_t alignment) override
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast)
        if (reinterpret_cast<std::uintptr_t>(ptr_) % alignment != 0) {
            throw std::logic_error("buffer is not properly aligned");
        }
        if (len_ == 0) {
            throw std::logic_error(
                "static_once_buffer can allocate only once");
        }
        if (bytes > len_) {
            throw std::length_error("not enough buffer is available");
        }
        len_ = 0;
        return ptr_;
    }

    void do_deallocate(void* p, std::size_t bytes,
                       std::size_t /*alignment*/) override
    {
        if (p != ptr_) {
            throw std::invalid_argument("bad pointer to deallocate");
        }
        len_ = bytes;
    }

    bool do_is_equal(
        const std::pmr::memory_resource& other) const noexcept override
    {
        return this == &other;
    }

    void* ptr_;
    std::size_t len_;
};
#endif

using char_array_8 = char[8];

DEFINE_STRUCT(        //
    S1,               //
    (int)v1,          //
    (short)v2,        //
    (char_array_8)v3, //
    (bool)flag        //
);

DEFINE_STRUCT(               //
    S2,                      //
    (int)v1,                 //
    (short)v2,               //
    (std::array<char, 8>)v3, //
    (float)v4,               //
    (bool)flag               //
);

template <typename T, typename = void>
struct naive_serializer {
    static_assert(std::is_standard_layout_v<T> &&
                  std::is_trivially_copyable_v<T>);

    template <typename SerializerList>
    static void serialize(const T& value, mozi::serialize_t& dest,
                          SerializerList /*unused*/)
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
        std::array<std::byte, sizeof(T)> buffer;
        std::memcpy(buffer.data(), &value, sizeof(T));
        dest.insert(dest.end(), buffer.begin(), buffer.end());
    }

    template <typename SerializerList>
    static deserialize_result deserialize(T& value,
                                          mozi::deserialize_t& src,
                                          SerializerList /*unused*/)
    {
        if (src.size() < sizeof(T)) {
            return deserialize_result::input_truncated;
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
        std::array<std::byte, sizeof(T)> buffer;
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            buffer[i] = src[i];
        }
        src = src.subspan(sizeof(T));
        std::memcpy(&value, buffer.data(), sizeof(T));
        return deserialize_result::success;
    }
};

template <typename T, typename = void>
struct naive_numbered_serializer {
    static_assert(std::is_standard_layout_v<T> &&
                  std::is_trivially_copyable_v<T>);

    template <typename SerializerList>
    static void serialize(const T& value, mozi::serialize_t& dest,
                          SerializerList /*unused*/, std::uint8_t& counter)
    {
        if (counter == UINT8_MAX) {
            throw std::runtime_error("too many items to serialize");
        }
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
        std::array<std::byte, sizeof(T)> buffer;
        std::memcpy(buffer.data(), &value, sizeof(T));
        dest.push_back(static_cast<std::byte>(counter));
        dest.insert(dest.end(), buffer.begin(), buffer.end());
        ++counter;
    }

    template <typename SerializerList>
    static deserialize_result
    deserialize(T& value, mozi::deserialize_t& src,
                SerializerList /*unused*/, std::uint8_t& counter)
    {
        if (counter == UINT8_MAX) {
            throw std::runtime_error("too many items to deserialize");
        }
        if (src.size() < sizeof(T) + 1) {
            return deserialize_result::input_truncated;
        }
        auto current_count = static_cast<std::uint8_t>(src[0]);
        if (current_count != counter) {
            return deserialize_result::unexpected_input_data;
        }
        ++counter;
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-member-init)
        std::array<std::byte, sizeof(T)> buffer;
        for (std::size_t i = 0; i < sizeof(T); ++i) {
            buffer[i] = src[i + 1];
        }
        src = src.subspan(sizeof(T) + 1);
        std::memcpy(&value, buffer.data(), sizeof(T));
        return deserialize_result::success;
    }
};

} // unnamed namespace

TEST_CASE("serialization: net_pack")
{
    using mozi::net_pack::serialize;
    using mozi::net_pack::deserialize;

    SECTION("basic types")
    {
        std::uint8_t value1{0x08};
        const std::uint16_t value2{0x1234};
        volatile std::uint32_t value3{0x12345678};
        mozi::serialize_t result;
        serialize(value1, result);
        serialize(value2, result);
        serialize(value3, result);
        serialize(true, result);
        serialize(false, result);
        serialize(std::byte(0xFF), result);

        std::uint8_t expected_result[]{0x08, 0x12, 0x34, 0x12, 0x34,
                                       0x56, 0x78, 0x01, 0x00, 0xFF};
        CHECK(mozi::equal(mozi::span<const std::byte>(result),
                          make_byte_span(expected_result)));

        std::uint8_t value4{};
        std::uint16_t value5{};
        std::uint32_t value6{};
        mozi::deserialize_t input{result};
        auto ec = deserialize(value4, input);
        REQUIRE(ec == deserialize_result::success);
        CHECK(value1 == value4);
        ec = deserialize(value5, input);
        REQUIRE(ec == deserialize_result::success);
        CHECK(value2 == value5);
        ec = deserialize(value6, input);
        REQUIRE(ec == deserialize_result::success);
        CHECK(value3 == value6);
        ec = deserialize(value6, input);
        CHECK(ec == deserialize_result::input_truncated);
    }

    SECTION("complex types")
    {
        S1 data{1, 2, {'H', 'e', 'l', 'l', 'o'}, true};
        auto result = serialize(data);
        std::uint8_t expected_result[]{0x00, 0x00, 0x00, 0x01, 0x00,
                                       0x02, 'H',  'e',  'l',  'l',
                                       'o',  0,    0,    0,    0x01};
        CHECK(mozi::equal(mozi::span<const std::byte>(result),
                          make_byte_span(expected_result)));

        mozi::deserialize_t input{result};
        S1 data2{};
        auto ec = deserialize(data2, input);
        REQUIRE(ec == deserialize_result::success);
        CHECK(input.empty());
        CHECK(mozi::equal(data, data2));
    }
}

TEST_CASE("serialization: multiple serializers")
{
    // Serialization for floats will fall back to naive_serializer
    mozi::serializer_list<mozi::net_pack::serializer, naive_serializer>
        serializers;

    S2 data{1, 2, {'H', 'e', 'l', 'l', 'o'}, 1.5, false};
    mozi::serialize_t result;
    // WON'T COMPILE: mozi::net_pack::serialize(data, result);
    mozi::serialize(data, result, serializers);
    mozi::deserialize_t input{result};
    S2 data2{};
    auto ec = mozi::deserialize(data2, input, serializers);
    REQUIRE(ec == deserialize_result::success);
    CHECK(input.empty());
    CHECK(mozi::equal(data, data2));
}

TEST_CASE("serialization: stateful serializer")
{
    mozi::serializer_list<naive_numbered_serializer> serializers;
    mozi::serialize_t result;
    uint8_t counter{};
    mozi::serialize(std::uint16_t(0x0012), result, serializers,
                    std::tuple(&counter));
    mozi::serialize(std::uint32_t(0x3456789a), result, serializers,
                    std::tuple(&counter));
    CHECK(counter == 2);
    REQUIRE(result.size() == 8);
    CHECK((result[0] == std::byte{0}));
    CHECK((result[1] == std::byte{0x00} || result[1] == std::byte{0x12}));
    CHECK((result[3] == std::byte{1}));
    CHECK((result[4] == std::byte{0x34} || result[4] == std::byte{0x9a}));

    counter = 0;
    mozi::deserialize_t input{result};
    std::uint16_t v1{};
    auto ec =
        mozi::deserialize(v1, input, serializers, std::tuple(&counter));
    CHECK(counter == 1);
    CHECK(ec == deserialize_result::success);
    CHECK(v1 == 0x0012);
    std::uint32_t v2{};
    ec = mozi::deserialize(v2, input, serializers, std::tuple(&counter));
    CHECK(counter == 2);
    CHECK(ec == deserialize_result::success);
    CHECK(v2 == 0x3456789a);
}

#if MOZI_SERIALIZATION_USES_PMR == 1
TEST_CASE("serialization: pmr buffer")
{
    S1 data{1, 2, {'H', 'e', 'l', 'l', 'o'}, false};
    std::byte buffer[100];
    static_once_buffer res(buffer, sizeof buffer);
    std::pmr::polymorphic_allocator<int> a(&res);
    {
        mozi::serialize_t result(a);
        result.reserve(100);
        mozi::net_pack::serialize(data, result);
        std::uint8_t expected_result[]{0x00, 0x00, 0x00, 0x01, 0x00,
                                       0x02, 'H',  'e',  'l',  'l',
                                       'o',  0,    0,    0,    0x00};
        CHECK(mozi::equal(mozi::span<const std::byte>(result),
                          make_byte_span(expected_result)));

        mozi::deserialize_t input{result};
        S1 data2{};
        auto ec = mozi::net_pack::deserialize(data2, input);
        REQUIRE(ec == deserialize_result::success);
        CHECK(input.empty());
        CHECK(mozi::equal(data, data2));
    }
    {
        mozi::serialize_t result(a);
        result.reserve(100);
        mozi::serialize(data, result,
                        mozi::serializer_list<mozi::net_pack::serializer>{},
                        std::tuple(nullptr));
        std::uint8_t expected_result[]{0x00, 0x00, 0x00, 0x01, 0x00,
                                       0x02, 'H',  'e',  'l',  'l',
                                       'o',  0,    0,    0,    0x00};
        CHECK(mozi::equal(mozi::span<const std::byte>(result),
                          make_byte_span(expected_result)));
    }
}
#endif
