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

#include "mozi/print.hpp"               // mozi/print/println
#include <array>                        // std::array
#include <cstddef>                      // std::byte
#include <cstdint>                      // std::uint8_t
#include <map>                          // std::map
#include <sstream>                      // std::ostringstream
#include <string>                       // std::string
#include <string_view>                  // std::string_view
#include <vector>                       // std::vector
#include <catch2/catch_test_macros.hpp> // Catch2 test macros

#if __cplusplus > 201703L && __has_include(<ranges>)
#include <ranges>
#endif

TEST_CASE("print")
{
    std::ostringstream oss;

    SECTION("basic types")
    {
        int n = 42;
        mozi::println(n, oss);
        char ch = 'H';
        mozi::println(ch, oss);
        CHECK(oss.str() == "42\n'H'\n");
    }

    SECTION("string types")
    {
        const char *msg1 = "Hello";
        char msg2[] = "world";
        std::string msg3{"from"};
        std::string_view msg4{"Mozi"};
        mozi::print(msg1, oss);
        mozi::print(msg2, oss);
        mozi::print(msg3, oss);
        mozi::print(msg4, oss);
        CHECK(oss.str() == "\"Hello\"\"world\"\"from\"\"Mozi\"");
    }

    SECTION("container types")
    {
        std::array a{1, 2, 3};
        std::byte b[]{std::byte{4}, std::byte{5}, std::byte{6}};
        std::vector<std::uint8_t> v{7, 8, 9};
        std::map<int, char> mp{{1, 'a'}, {2, 'b'}, {3, 'c'}};
        mozi::println(a, oss);
        mozi::println(b, oss);
        mozi::println(v, oss);
        mozi::println(mp, oss);
        CHECK(oss.str() == "{ 1, 2, 3 }\n"
                           "{ 4, 5, 6 }\n"
                           "{ 7, 8, 9 }\n"
                           "{ 1 => 'a', 2 => 'b', 3 => 'c' }\n");
    }
}

#ifdef __cpp_lib_ranges
TEST_CASE("print c++20 ranges")
{
    std::vector v{1, 2, 3, 4, 5, 6};
    std::ostringstream oss;
    mozi::print(v | std::views::filter([](int n) { return n % 2 == 0; }) |
                    std::views::reverse,
                oss);
    CHECK(oss.str() == "{ 6, 4, 2 }");
}
#endif
