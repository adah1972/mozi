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

#include "mozi/bit_fields.hpp"          // mozi::bit_field/...
#include <ios>                          // std::hex
#include <sstream>                      // std::ostringstream
#include <catch2/catch_test_macros.hpp> // Catch2 test macros
#include "mozi/copy.hpp"                // mozi::copy
#include "mozi/equal.hpp"               // mozi::equal
#include "mozi/print.hpp"               // mozi::println
#include "mozi/struct_reflection.hpp"   // DEFINE_STRUCT

namespace {

DEFINE_BIT_FIELDS_CONTAINER(                           //
    Date,                                              //
    (mozi::bit_field<23, mozi::bit_field_signed>)year, //
    (mozi::bit_field<4>)month,                         //
    (mozi::bit_field<5>)day                            //
);

DECLARE_COMPARISON(Date);

DEFINE_BIT_FIELDS_CONTAINER(  //
    Flags,                    //
    (mozi::bit_field<3>)f1,  //
    (mozi::bit_field<17>)f2, //
    (mozi::bit_field<12>)f3  //
);

DEFINE_STRUCT( //
    S1,        //
    (int)v1,   //
    (short)v2, //
    (Flags)v3  //
);

DEFINE_STRUCT( //
    S2,        //
    (int)v1,   //
    (int)v2,   //
    (Flags)v3  //
);

} // unnamed namespace

TEST_CASE("bit_fields: basic")
{
    Date d1{2023, 12, 31};
    Date d2{2024, 1, 13};
    CHECK(d1 == d1);
    CHECK(d1 < d2);
    CHECK(d1 <= d2);
    CHECK(d2 > d1);
    CHECK(!(d2 < d1));
    d2.year = -1;
    CHECK(d2.year == -1);
    CHECK(d2.year != 0);
    CHECK(d2.month == 1);
    CHECK(d2.day == 13);
    CHECK(d2 < d1);

    d2.day = 33;
    CHECK(d2.day != 33);
    mozi::bit_field<3, mozi::bit_field_signed> f1{3};
    CHECK(f1 == 3);
    f1 = -4;
    CHECK(f1 == -4);
    f1 = 4;
    CHECK(f1 != 4);
    f1 = -5;
    CHECK(f1 != -5);
    mozi::bit_field<9, mozi::bit_field_signed> f2{-2};
    CHECK(f2 == -2);
    mozi::bit_field<19, mozi::bit_field_signed> f3{-3};
    CHECK(f3 == -3);
    mozi::bit_field<32, mozi::bit_field_signed> f4{-4};
    CHECK(f4 == -4);
}

TEST_CASE("bit_fields: print")
{
    std::ostringstream oss;
    mozi::bit_field<4> f1{1};
    mozi::println(f1, oss);
#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif
    f1 = -2; // warning expected here
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
    mozi::println(f1, oss);
    mozi::bit_field<4, mozi::bit_field_signed> f2{-3};
    mozi::println(f2, oss);
    CHECK(oss.str() == "1\n14\n-3\n");

    oss.str("");
    Date d{2024, 8, 17};
    mozi::println(d, oss);
    CHECK(oss.str() == "{\n"
                       "    year: 2024,\n"
                       "    month: 8,\n"
                       "    day: 17\n"
                       "}\n");

    oss.str("");
    S1 data{42, 0x1234, {{1}, {0x1FFFF}, {0b101010101010}}};
    oss << std::hex;
    mozi::println(data, oss);
    CHECK(oss.str() == "{\n"
                       "    v1: 2a,\n"
                       "    v2: 1234,\n"
                       "    v3: {\n"
                       "        f1: 1,\n"
                       "        f2: 1ffff,\n"
                       "        f3: aaa\n"
                       "    }\n"
                       "}\n");
}

TEST_CASE("bit_fields: copy")
{
    S1 data{42, 0x1234, {{1}, {0x1FFFF}, {0b101010101010}}};
    S2 data2{};
    mozi::copy(data, data2);
    CHECK(mozi::equal(data, data2));
    CHECK(mozi::equal(data2, data));
    data2.v3.f1 = 0b010;
    CHECK(!mozi::equal(data, data2));
    CHECK(!mozi::equal(data2, data));
}
