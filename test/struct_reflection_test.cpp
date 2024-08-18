/*
 * Copyright (c) 2023-2024 Wu Yongwei
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

#include "mozi/struct_reflection.hpp"   // DEFINE_STRUCT
#include <ios>                          // std::boolalpha
#include <map>                          // std::map
#include <sstream>                      // std::ostringstream
#include <string>                       // std::string
#include <tuple>                        // std::tuple
#include <type_traits>                  // std::decay/is_integral
#include <utility>                      // std::move
#include <vector>                       // std::vector
#include <stdint.h>                     // uint16_t/uint32_t
#include <catch2/catch_test_macros.hpp> // Catch2 test macros
#include "mozi/copy.hpp"                // mozi::copy
#include "mozi/equal.hpp"               // mozi::equal
#include "mozi/print.hpp"               // mozi::print/println

#if __has_include(<boost/pfr.hpp>)
#include <boost/pfr/core.hpp>           // boost::pfr::structure_tie

namespace {

struct NormalStruct {
    uint16_t v1;
    uint16_t v2;
    uint32_t v3;
    uint32_t v4;
    std::string msg;
};

} // unnamed namespace
#endif

namespace data {

using mozi::get; // Necessary for argument-dependent lookup

DEFINE_STRUCT(       //
    S1,              //
    (uint16_t)v1,    //
    (uint16_t)v2,    //
    (uint32_t)v3,    //
    (uint32_t)v4,    //
    (std::string)msg //
);

DECLARE_COMPARISON(S1);

DEFINE_STRUCT( //
    S2,        //
    (int)v2,   //
    (long)v4,  //
    (bool)flag //
);

DEFINE_STRUCT(               //
    S3,                      //
    (std::vector<S2>)values, //
    (std::string)msg         //
);

DEFINE_STRUCT( //
    S4,        //
    (int)v2,   //
    (int)v4,  //
    (bool)flag //
);

DEFINE_STRUCT( //
    S5,        //
    (int)v2,   //
    (long)v4   //
);

} // namespace data

DECLARE_TUPLE_LIKE(data::S1);
DECLARE_TUPLE_LIKE(data::S2);
DECLARE_TUPLE_LIKE(data::S3);

namespace {

template <typename T>
constexpr long add_all_fields(const T& obj)
{
    long result{};
    mozi::for_each(obj, [&](auto /*index*/, auto /*name*/,
                            const auto& value) {
        if constexpr (std::is_integral_v<std::decay_t<decltype(value)>>) {
            result += value;
        }
    });
    return result;
}

} // unnamed namespace

TEST_CASE("struct_reflection: for_each")
{
    data::S1 s1{1, 2, 3, 4, "Hello"};
    CHECK(add_all_fields(s1) == 10);

    SECTION("add_all_fields")
    {
        constexpr data::S2 s2{2, 4, true};
        constexpr auto result = add_all_fields(s2);
        CHECK(result == 7);
    }

    SECTION("copy_same_name_fields")
    {
        data::S2 s2{};
        mozi::copy_same_name_fields<mozi::missing_fields{1}>(s1, s2);
        CHECK(s2.v2 == s1.v2);
        CHECK(s2.v4 == s1.v4);
    }
}

TEST_CASE("struct_reflection: print")
{
    using data::S1;
    using data::S2;
    using data::S3;
    std::ostringstream oss;
    oss << std::boolalpha;

    SECTION("basic")
    {
        S2 s2{0, 1, false};
        mozi::println(s2, oss);
        CHECK(oss.str() == "{\n"
                           "    v2: 0,\n"
                           "    v4: 1,\n"
                           "    flag: false\n"
                           "}\n");
    }

    SECTION("nested in tuple")
    {
        auto obj = std::tuple{S1{1, 2, 3, 4, "Hello"}, 42, "universe"};
        mozi::println(obj, oss);
        CHECK(oss.str() == "({\n"
                           "    v1: 1,\n"
                           "    v2: 2,\n"
                           "    v3: 3,\n"
                           "    v4: 4,\n"
                           "    msg: \"Hello\"\n"
                           "}, 42, \"universe\")\n");
    }

    SECTION("nested in vector")
    {
        S3 s3{{{0, 1, false}, {2, 3, true}}, "testing"};
        mozi::println(s3, oss);
        CHECK(oss.str() == "{\n"
                           "    values: {\n"
                           "        {\n"
                           "            v2: 0,\n"
                           "            v4: 1,\n"
                           "            flag: false\n"
                           "        },\n"
                           "        {\n"
                           "            v2: 2,\n"
                           "            v4: 3,\n"
                           "            flag: true\n"
                           "        }\n"
                           "    },\n"
                           "    msg: \"testing\"\n"
                           "}\n");
    }

    SECTION("nested in map")
    {
        std::map<std::string, S3> mp{
            {"empty", {}},
            {"short", {{{0, 1, false}}, "testing"}},
            {"long", {{{0, 1, false}, {2, 3, true}}, "testing"}},
        };
        S3 s3{{{0, 1, false}, {2, 3, true}}, "testing"};
        mozi::println(mp, oss);
        CHECK(oss.str() == "{\n"
                           "    \"empty\" => {\n"
                           "        values: {},\n"
                           "        msg: \"\"\n"
                           "    },\n"
                           "    \"long\" => {\n"
                           "        values: {\n"
                           "            {\n"
                           "                v2: 0,\n"
                           "                v4: 1,\n"
                           "                flag: false\n"
                           "            },\n"
                           "            {\n"
                           "                v2: 2,\n"
                           "                v4: 3,\n"
                           "                flag: true\n"
                           "            }\n"
                           "        },\n"
                           "        msg: \"testing\"\n"
                           "    },\n"
                           "    \"short\" => {\n"
                           "        values: {\n"
                           "            {\n"
                           "                v2: 0,\n"
                           "                v4: 1,\n"
                           "                flag: false\n"
                           "            }\n"
                           "        },\n"
                           "        msg: \"testing\"\n"
                           "    }\n"
                           "}\n");
    }
}

TEST_CASE("struct_reflection: copy to/from tuple")
{
    data::S1 s1{1, 2, 3, 4, "a long string that defeats SSO"};

    SECTION("normal tuple")
    {
        std::tuple<uint16_t, uint16_t, uint16_t, uint16_t, std::string> tup;
        mozi::copy(s1, tup);
        CHECK(s1.v1 == std::get<0>(tup));
        CHECK(s1.v2 == std::get<1>(tup));
        CHECK(s1.v3 == std::get<2>(tup));
        CHECK(s1.v4 == std::get<3>(tup));
        CHECK(s1.msg == std::get<4>(tup));

        s1 = {};
        REQUIRE(s1.v1 != std::get<0>(tup));
        mozi::copy(std::move(tup), s1);
        CHECK(s1.v1 == std::get<0>(tup));
        CHECK(s1.v2 == std::get<1>(tup));
        CHECK_FALSE(s1.msg.empty());
        CHECK(std::get<4>(tup).empty());
    }

#if __has_include(<boost/pfr.hpp>)
    SECTION("Boost.PFR")
    {
        NormalStruct s2{};

        auto target = boost::pfr::structure_tie(s2);
        // Using a (tuple) variable is necessary as the copy target
        mozi::copy(std::move(s1), target);
        // NOLINTBEGIN(bugprone-use-after-move)
        CHECK(s1.v1 == s2.v1);
        CHECK(s1.v2 == s2.v2);
        CHECK(s1.v3 == s2.v3);
        CHECK(s1.v4 == s2.v4);
        CHECK(s1.msg.empty());
        // NOLINTEND(bugprone-use-after-move)
        CHECK_FALSE(s2.msg.empty());

        s1 = {};
        REQUIRE(s1.v1 != s2.v1);
        // N.B.: structure_tie does not work on rvalues
        mozi::copy(boost::pfr::structure_tie(s2), s1);
        CHECK(s1.v1 == s2.v1);
        CHECK(s1.v2 == s2.v2);
        CHECK(s1.msg == s2.msg);
    }
#endif
}

TEST_CASE("struct_reflection: compare")
{
    using S = data::S1;
    S s1{1, 2, 3, 4, "Hello"};
    S s2{1, 2, 3, 4, "Hello"};
    S s3{1, 2, 3, 4, ""};
    S s4{1, 2, 3, 0, "Hello"};
    CHECK(mozi::equal(s1, s2));
    CHECK(s1 == s2);
    CHECK_FALSE(mozi::equal(s1, s3));
    CHECK_FALSE(mozi::equal(s1, s4));
    CHECK(s1 != s3);
    CHECK(s1 != s4);

    CHECK(s1 > s3);
    CHECK(s1 >= s3);
    CHECK(s3 > s4);
    CHECK(s4 <= s3);
}

TEST_CASE("struct_reflection: equal")
{
    // Reflected structs of different sizes are never equal
    constexpr data::S2 s2{};
    constexpr data::S5 s5{};
    constexpr bool result = mozi::equal(s2, s5);
    CHECK_FALSE(result);

    // Reflected structs of the same size may be equal
    data::S2 s21{1, 2, true};
    data::S4 s41{1, 2, true};
    data::S4 s42{1, 2, false};
    CHECK(mozi::equal(s21, s41));
    CHECK_FALSE(mozi::equal(s2, s21));
    CHECK_FALSE(mozi::equal(s21, s42));
}
