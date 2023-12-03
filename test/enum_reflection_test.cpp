/*
 * Copyright (c) 2023 Wu Yongwei
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

#include "mozi/enum_reflection.hpp"     // DEFINE_ENUM/DEFINE_ENUM_CLASS
#include <sstream>                      // std::ostringstream
#include <tuple>                        // std::tuple
#include <type_traits>                  // std::is_enum
#include <vector>                       // std::vector
#include <stdint.h>                     // uint8_t
#include <catch2/catch_test_macros.hpp> // Catch2 test macros
#include "mozi/print.hpp"               // mozi::print
#include "mozi/type_traits.hpp"         // mozi::is_reflected_enum

namespace {

DEFINE_ENUM(           //
    Channel, unsigned, //
    CHANNEL_RED, CHANNEL_GREEN, CHANNEL_BLUE, CHANNEL_ALPHA);

DEFINE_ENUM_CLASS(  //
    Color, uint8_t, //
    red = 1, highlight = Color::red, green, blue);

enum class Number { zero, one, two, three };

} // unnamed namespace

using mozi::enum_to_string;

TEST_CASE("enum_reflection: enum")
{
    SECTION("is_enum", "reflected enum is enum")
    {
        CHECK(std::is_enum_v<Channel>);
    }

    SECTION("is_defined", "check defined enumerators")
    {
        CHECK(is_defined(CHANNEL_RED));
        CHECK(is_defined(CHANNEL_GREEN));
        CHECK(is_defined(CHANNEL_BLUE));
        CHECK(is_defined(CHANNEL_ALPHA));
        CHECK_FALSE(is_defined(Channel{9}));
    }

    SECTION("to_string", "converts enumerator to string")
    {
        CHECK(to_string(CHANNEL_RED) == "CHANNEL_RED");
        CHECK(to_string(CHANNEL_GREEN) == "CHANNEL_GREEN");
        CHECK(to_string(CHANNEL_BLUE) == "CHANNEL_BLUE");
        CHECK(to_string(Channel{9}) == "(Channel)9");
    }

    Channel channel{};
    SECTION("from_string", "convert string to enumerator")
    {
        REQUIRE(from_string("CHANNEL_RED", channel));
        CHECK(channel == CHANNEL_RED);
    }
}

TEST_CASE("enum_reflection: enum class")
{
    SECTION("is_enum", "reflected enum is enum")
    {
        CHECK(std::is_enum_v<Color>);
    }

    SECTION("is_defined", "check defined enumerators")
    {
        CHECK(is_defined(Color::red));
        CHECK(is_defined(Color::highlight));
        CHECK(is_defined(Color::green));
        CHECK(is_defined(Color::blue));
        CHECK_FALSE(is_defined(Color{0}));
        CHECK_FALSE(is_defined(Color{9}));
    }

    Color color{1};
    SECTION("to_string", "converts enumerator to string")
    {
        CHECK(to_string(color, enum_to_string::show_name) == "Color::red");
        CHECK(to_string(Color::highlight) == "red");
        CHECK(to_string(Color::green) == "green");
        CHECK(to_string(Color::blue) == "blue");
        CHECK(to_string(Color{9}) == "(Color)9");
    }

    SECTION("from_string", "convert string to enumerator")
    {
        REQUIRE(from_string("highlight", color));
        CHECK(color == Color::highlight);
    }
}

TEST_CASE("enum_reflection: reflected?")
{
    CHECK(mozi::is_reflected_enum_v<Channel>);
    CHECK(mozi::is_reflected_enum_v<Color>);
    CHECK_FALSE(mozi::is_reflected_enum_v<Number>);
}

TEST_CASE("enum_reflection: print")
{
    std::ostringstream oss;

    SECTION("enum")
    {
        mozi::print(Channel::CHANNEL_BLUE, oss);
        CHECK(oss.str() == "CHANNEL_BLUE");
    }

    SECTION("enum class")
    {
        mozi::print(Color::blue, oss);
        CHECK(oss.str() == "Color::blue");
    }

    SECTION("enum in other data structures")
    {
        std::tuple tup{Color::green, std::vector{CHANNEL_RED, CHANNEL_BLUE},
                       3};
        mozi::print(tup, oss);
        CHECK(oss.str() ==
              "(Color::green, { CHANNEL_RED, CHANNEL_BLUE }, 3)");
    }
}
