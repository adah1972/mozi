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

#ifndef MOZI_ENUM_REFLECTION_CORE_HPP
#define MOZI_ENUM_REFLECTION_CORE_HPP

#include <algorithm>   // std::lower_bound/sort/stable_sort/unique
#include <array>       // IWYU pragma: keep std::array
#include <cstddef>     // std::size_t
#include <string>      // std::string
#include <string_view> // std::string_view
#include <type_traits> // std::decay/underlying_type/void_t/...
#include <utility>     // std::pair
#include <vector>      // std::vector
#include "metamacro.h" // MOZI_GET_ARG_COUNT/MOZI_REPEAT_FIRST_ON

namespace mozi {

enum class enum_to_string { no_show_name, show_name };

namespace detail {

template <typename Enum>
class eat_assign {
public:
    constexpr explicit eat_assign(Enum value) : value_(value) {}

    template <typename Any>
    constexpr const eat_assign& operator=(Any) const // NOLINT
    {
        return *this;
    }

    constexpr operator Enum() const
    {
        return value_;
    }

private:
    Enum value_;
};

template <typename Int>
using enum_value_name_pair = std::pair<Int, std::string_view>;

template <typename Int>
struct enum_value_comparator {
    bool operator()(const enum_value_name_pair<Int>& lhs,
                    const enum_value_name_pair<Int>& rhs) const
    {
        return lhs.first < rhs.first;
    }
    bool operator()(Int lhs, const enum_value_name_pair<Int>& rhs) const
    {
        return lhs < rhs.first;
    }
    bool operator()(const enum_value_name_pair<Int>& lhs, Int rhs) const
    {
        return lhs.first < rhs;
    }
};

template <typename Int>
struct enum_name_comparator {
    bool operator()(const enum_value_name_pair<Int>& lhs,
                    const enum_value_name_pair<Int>& rhs) const
    {
        return lhs.second < rhs.second;
    }
    bool operator()(std::string_view lhs,
                    const enum_value_name_pair<Int>& rhs) const
    {
        return lhs < rhs.second;
    }
    bool operator()(const enum_value_name_pair<Int>& lhs,
                    std::string_view rhs) const
    {
        return lhs.second < rhs;
    }
};

// NOLINTNEXTLINE(bugprone-exception-escape)
constexpr std::string_view remove_equals(std::string_view s) noexcept
{
    std::size_t pos = s.find('=');
    if (pos != std::string_view::npos) {
        while (s[pos - 1] == ' ') {
            --pos;
        }
        return s.substr(0, pos);
    }
    return s;
}

template <typename Int, typename Iterator>
auto sort_uniq_first(Iterator first, Iterator last)
{
    std::vector<std::decay_t<decltype(*first)>> result{first, last};
    std::stable_sort(result.begin(), result.end(),
                     [](const auto& lhs, const auto& rhs) {
                         return lhs.first < rhs.first;
                     });
    auto end = std::unique(result.begin(), result.end(),
                           [](const auto& lhs, const auto& rhs) {
                               return lhs.first == rhs.first;
                           });
    result.erase(end, result.end());
    return result;
}

template <typename Int, typename Iterator>
auto sort_second(Iterator first, Iterator last)
{
    std::vector<std::decay_t<decltype(*first)>> result{first, last};
    std::sort(result.begin(), result.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs.second < rhs.second;
              });
    return result;
}

template <typename Int, typename Iterator>
Iterator find_enum_value(Int value, Iterator first, Iterator last)
{
    enum_value_comparator<Int> compare;
    Iterator it = std::lower_bound(first, last, value, compare);
    if (it != last && !compare(value, *it)) {
        return it;
    }
    return last;
}

template <typename Int, typename Iterator>
std::string to_string_impl(Int value, const char* enum_name,
                           enum_to_string flag, Iterator first,
                           Iterator last)
{
    std::string result;
    auto it = find_enum_value(value, first, last);
    if (it != last) {
        if (flag == enum_to_string::show_name) {
            result = enum_name;
            result += "::";
        }
        result += it->second;
    } else {
        result = "(";
        result += enum_name;
        result += ")";
        result += std::to_string(value);
    }
    return result;
}

template <typename Enum, typename Iterator>
bool from_string_impl(std::string_view name, Enum& value, Iterator first,
                      Iterator last)
{
    enum_name_comparator<std::underlying_type_t<Enum>> compare;
    auto it = std::lower_bound(first, last, name, compare);
    if (it != last && !compare(name, *it)) {
        value = Enum{it->first};
        return true;
    }
    return false;
}

} // namespace detail

template <typename Enum, typename Iterator>
const std::vector<
    detail::enum_value_name_pair<std::underlying_type_t<Enum>>>&
get_enum_value_name_map(Iterator first, Iterator last)
{
    static auto result =
        detail::sort_uniq_first<std::underlying_type_t<Enum>>(first, last);
    return result;
}

template <typename Enum, typename Iterator>
const std::vector<
    detail::enum_value_name_pair<std::underlying_type_t<Enum>>>&
get_enum_name_value_map(Iterator first, Iterator last)
{
    static auto result =
        detail::sort_second<std::underlying_type_t<Enum>>(first, last);
    return result;
}

template <typename Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(e);
}

} // namespace mozi

#if defined(__GNUC__)
#define MOZI_DIAGNOSTIC_PUSH _Pragma("GCC diagnostic push")
#define MOZI_DIAGNOSTIC_POP _Pragma("GCC diagnostic pop")
#if defined(__clang__)
#define MOZI_DISABLE_UNUSED_FUNC_WARNING                                   \
    _Pragma("GCC diagnostic ignored \"-Wunneeded-internal-declaration\"")
#else
#define MOZI_DISABLE_UNUSED_FUNC_WARNING                                   \
    _Pragma("GCC diagnostic ignored \"-Wunused-function\"")
#endif
#else
#define MOZI_DIAGNOSTIC_PUSH
#define MOZI_DIAGNOSTIC_POP
#define MOZI_DISABLE_UNUSED_FUNC_WARNING
#endif

#define MOZI_ENUM_ITEM(i, e, arg)                                          \
    std::pair{mozi::to_underlying(                                         \
                  e((mozi::detail::eat_assign<e>)e::arg /* NOLINT */)),    \
              mozi::detail::remove_equals(#arg)},

#define MOZI_ENUM_FUNCTIONS(e, u)                                          \
    inline bool is_defined(e value)                                        \
    {                                                                      \
        const auto& value_name_map = mozi::get_enum_value_name_map<e>(     \
            e##_enum_map_.begin(), e##_enum_map_.end());                   \
        return mozi::detail::find_enum_value(                              \
                   mozi::to_underlying(value), value_name_map.begin(),     \
                   value_name_map.end()) != value_name_map.end();          \
    }                                                                      \
    inline std::string to_string(e value,                                  \
                                 mozi::enum_to_string flag =               \
                                     mozi::enum_to_string::no_show_name)   \
    {                                                                      \
        const auto& value_name_map = mozi::get_enum_value_name_map<e>(     \
            e##_enum_map_.begin(), e##_enum_map_.end());                   \
        return mozi::detail::to_string_impl(                               \
            mozi::to_underlying(value), #e, flag, value_name_map.begin(),  \
            value_name_map.end());                                         \
    }                                                                      \
    inline bool from_string(std::string_view name, e& value) /* NOLINT */  \
    {                                                                      \
        const auto& name_value_map = mozi::get_enum_name_value_map<e>(     \
            e##_enum_map_.begin(), e##_enum_map_.end());                   \
        return mozi::detail::from_string_impl(                             \
            name, value, name_value_map.begin(), name_value_map.end());    \
    }                                                                      \
    MOZI_DIAGNOSTIC_PUSH                                                   \
    MOZI_DISABLE_UNUSED_FUNC_WARNING                                       \
    bool is_mozi_reflected_enum(e) /* for is_reflected_enum only */;       \
    MOZI_DIAGNOSTIC_POP

#define MOZI_DEFINE_ENUM(e, u, ...)                                        \
    enum e : u { __VA_ARGS__ };                                            \
    inline constexpr std::array<std::pair<u, std::string_view>,            \
                                MOZI_GET_ARG_COUNT(__VA_ARGS__)>           \
        e##_enum_map_{                                                     \
            MOZI_REPEAT_FIRST_ON(MOZI_ENUM_ITEM, e, __VA_ARGS__)};         \
    MOZI_ENUM_FUNCTIONS(e, u)

#define MOZI_DEFINE_ENUM_CLASS(e, u, ...)                                  \
    enum class e : u { __VA_ARGS__ };                                      \
    inline constexpr std::array<std::pair<u, std::string_view>,            \
                                MOZI_GET_ARG_COUNT(__VA_ARGS__)>           \
        e##_enum_map_{                                                     \
            MOZI_REPEAT_FIRST_ON(MOZI_ENUM_ITEM, e, __VA_ARGS__)};         \
    MOZI_ENUM_FUNCTIONS(e, u)

#if !defined(DEFINE_ENUM) && !defined(MOZI_NO_DEFINE_ENUM)
#define DEFINE_ENUM MOZI_DEFINE_ENUM
#endif

#if !defined(DEFINE_ENUM_CLASS) && !defined(MOZI_NO_DEFINE_ENUM_CLASS)
#define DEFINE_ENUM_CLASS MOZI_DEFINE_ENUM_CLASS
#endif

#endif // MOZI_ENUM_REFLECTION_CORE_HPP
