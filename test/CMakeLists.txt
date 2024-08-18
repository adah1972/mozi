cmake_minimum_required(VERSION 3.10)
project(mozi_tests)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU AND
    CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10.0)
  set(CMAKE_CXX_STANDARD 20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang AND
    CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 15.0)
  set(CMAKE_CXX_STANDARD 20)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL AppleClang AND
    CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 14.0)
  set(CMAKE_CXX_STANDARD 20)
elseif(MSVC)
  set(CMAKE_CXX_STANDARD 20)
  add_compile_options(/Zc:__cplusplus)
else()
  set(CMAKE_CXX_STANDARD 17)
endif()

if(MSVC)
  add_compile_options(/W3)
else()
  add_compile_options(-Wall -Wextra -Wsign-compare -Wsign-conversion)
endif()

find_package(Catch2 3 REQUIRED)

include_directories(..)
add_executable(mozi_test
  enum_reflection_test.cpp
  struct_reflection_test.cpp
  equal_test.cpp
  print_test.cpp
  serialization_test.cpp
  bit_fields_test.cpp
)
target_link_libraries(mozi_test PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(mozi_test)
