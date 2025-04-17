Mozi
====

The Name
--------

[Mozi][lnk_mozi] was a renowned Chinese philosopher and craftsman, and
his name sounds very good for a programming project.

While it is OK to pronounce “Mozi” in any way you like (the proper
Chinese pronunciation is probably too difficult for most of the
English-speaking community), my preferred English pronunciation is
“MO-tsee” (/ˈməʊtsiː/).


The Project
-----------

This project serves as a continuation and successor to my previous C++
utility project, [Nvwa][lnk_nvwa], which is showing its age.  However, I
do not want to make dramatic changes to its code and break backward
compatibility, so a new project is more appropriate for new stuff.

In creating Mozi, I am also taking this opportunity to leave behind some
bad habits, such as the use of prefix-less macros and reserved
identifiers (e.g. `_NOTHREADS` and `_M_line`).  Overall, Mozi will have
a more modern style.


Base C++ Standard
-----------------

This project primarily uses the C++17 standard.  I need `string_view`s,
fold expressions, inline variables, and constexpr if statements.  They
are necessary for code readability and performance.

Where appropriate, support for newer C++ standards is added.


Contents
--------

Mozi is supposed to evolve over time.  Initially, manual static
reflection for `enum`s and `struct`s was included, as well as macros
that were necessary in such code generation usage.  Support facilities
were then added, like equality comparison and serialization.

A basic example of static reflection follows:

```cpp
DEFINE_STRUCT(
    S,
    (int)v1,
    (std::vector<uint8_t>)v2,
    (std::string)msg
);

S data{42, {0x01, 0x02}, "Hello"};
mozi::println(data);
```

The resulting output would be:

```
{
    v1: 42,
    v2: { 1, 2 },
    msg: "Hello"
}
```

More may come later.


Test Code
---------

The test code serves the purpose of both *verifying* and *demonstrating*
the functionalities of Mozi.  It is the only thing that requires
building.

In order to build the test, you need to have [CMake][lnk_cmake] and
[Catch2][lnk_catch2] v3.

### Prerequisite: CMake

You need to have CMake 3.10 or later.  Standard CMake installations
should usually do.

### Prerequisite: Catch2 v3

You need to have Catch2 v3, while many Linux distros only come with v2.
You probably need to follow the standard build and installation
procedure on such distros:

```
cd /path/to/Catch2/source
mkdir build && cd build
cmake ..
cmake --build . -j
sudo make install
```

I use [Homebrew][lnk_brew] on macOS (`brew install catch2`) and
[vcpkg][lnk_vcpkg] on Windows (`vcpkg install catch2`).  They have the
up-to-date Catch2 version.

### Tested compilers

I mainly test with the following compilers:

- GCC 10 and later on Linux
- Clang 17 and Apple Clang 14 on macOS
- MSVC as in Visual Studio 2022 on Windows

If you do not use any of these compilers and encounter a problem, you
had better report the problem with a patch.  I may not be able to verify
and fix your problem.

### Build and test

The following procedure should normally work:

```
cd /path/to/mozi/test
mkdir build && cd build
cmake ..
cmake --build . -j && ctest
```

You should then see the message “100% tests passed”.

#### Note to Windows users

Keep in mind that when using vcpkg you generally need to specify the
path to it in the third step above, like:

```
cmake "-DCMAKE_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake" ..
```


Related Speeches and Articles
-----------------------------

- [C++ 及系统软件技术大会 2023：C++ 之静态反射（演讲 PDF）](https://github.com/adah1972/cpp_summit_2023/)
- [PureCPP 大会 2024：使用 Mozi 库的静态反射：调试输出和序列化（演讲 PDF 和视频）](http://purecpp.cn/detail?id=2457)
- [Static reflection in C++ (blog article)](https://yongweiwu.wordpress.com/2024/12/28/static-reflection-in-cxx/)
- [Bit fields, byte order, and serialization (blog article)](https://yongweiwu.wordpress.com/2025/02/21/bit-fields-byte-order-and-serialization/)


[lnk_brew]:   https://brew.sh/
[lnk_catch2]: https://github.com/catchorg/Catch2
[lnk_cmake]:  https://cmake.org/
[lnk_mozi]:   https://en.wikipedia.org/wiki/Mozi
[lnk_nvwa]:   https://github.com/adah1972/nvwa
[lnk_vcpkg]:  https://github.com/microsoft/vcpkg


<!--
vim:autoindent:expandtab:formatoptions=tcqlm:textwidth=72:
-->
