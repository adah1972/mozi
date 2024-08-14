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

#include "mozi/equal.hpp"               // mozi::equal
#include <array>                        // std::array
#include <string_view>                  // std::string_view
#include <vector>                       // std::vector
#include <catch2/catch_test_macros.hpp> // Catch2 test macros

TEST_CASE("equal")
{
    SECTION("standard types")
    {
        long l = 42;
        CHECK(mozi::equal(l, 42));

        std::string_view sv{"hello"};
        CHECK(mozi::equal(sv, "hello"));
        CHECK_FALSE(mozi::equal(sv, "Hell"));

        std::vector v1{1, 2, 3};
        std::vector v2 = v1;
        std::vector v3 = v1;
        v3[0] = 0;
        CHECK(mozi::equal(v1, v2));
        CHECK_FALSE(mozi::equal(v1, v3));
    }

    SECTION("arrays")
    {
        int a1[]{1, 2, 3};
        int a2[]{1, 2, 3};
        CHECK(mozi::equal(a1, a2));
        a1[0] = 0;
        CHECK_FALSE(mozi::equal(a1, a2));

        std::array<int, 3> a3{1, 2, 3};
        std::array<int, 3> a4 = a3;
        CHECK(mozi::equal(a3, a4));
        a3[0] = 0;
        CHECK_FALSE(mozi::equal(a3, a4));
    }
}
