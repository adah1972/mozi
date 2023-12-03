#include "mozi/struct_reflection.hpp"   // DEFINE_STRUCT
#include <cstddef>                      // std::size_t
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

} // namespace data

DECLARE_TUPLE_LIKE(data::S1);
DECLARE_TUPLE_LIKE(data::S2);
DECLARE_TUPLE_LIKE(data::S3);

namespace {

template <typename T>
constexpr long add_all_fields(const T& obj)
{
    long result{};
    mozi::for_each(obj, [&](std::size_t /*index*/, auto /*name*/,
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
        CHECK(s1.v1 == s2.v1);
        CHECK(s1.v2 == s2.v2);
        CHECK(s1.v3 == s2.v3);
        CHECK(s1.v4 == s2.v4);
        CHECK(s1.msg.empty());
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
