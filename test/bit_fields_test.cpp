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

TEST_CASE("bit_fields: print")
{
    S1 data{42, 0x1234, {{1}, {0x1FFFF}, {0b101010101010}}};
    std::ostringstream oss;
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

TEST_CASE("bit_fields: copy & equal")
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
