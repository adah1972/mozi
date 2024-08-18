/*
 * Copyright (c) 2021 Netcan
 * Copyright (c) 2021-2023 Wu Yongwei
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

#ifndef MOZI_METAMACRO_H
#define MOZI_METAMACRO_H

#define MOZI_PP_3RD_ARG(a, b, c, ...) c
#define MOZI_VA_OPT_SUPPORTED_I(...) MOZI_PP_3RD_ARG(__VA_OPT__(, ), 1, 0, )
#define MOZI_VA_OPT_SUPPORTED MOZI_VA_OPT_SUPPORTED_I(?)

// Traditional MSVC requires a special EXPAND phase
#if (defined(_MSC_VER) && !defined(_MSVC_TRADITIONAL)) ||                  \
    (defined(_MSVC_TRADITIONAL) && _MSVC_TRADITIONAL)

#define MOZI_GET_ARG_COUNT(...)                                            \
    MOZI_INTERNAL_EXPAND_ARGS_PRIVATE(                                     \
        MOZI_INTERNAL_ARGS_AUGMENTER(__VA_ARGS__))

#define MOZI_INTERNAL_ARGS_AUGMENTER(...) unused, __VA_ARGS__
#define MOZI_INTERNAL_EXPAND_ARGS_PRIVATE(...)                             \
    MOZI_EXPAND(MOZI_INTERNAL_GET_ARG_COUNT_PRIVATE(                       \
        __VA_ARGS__, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91,    \
        90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75,    \
        74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59,    \
        58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43,    \
        42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27,    \
        26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11,    \
        10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define MOZI_REPEAT_0(func, i, arg)
#define MOZI_REPEAT_1(func, i, arg) \
    func(i, arg)
#define MOZI_REPEAT_2(func, i, arg, ...)                                   \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_1(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_3(func, i, arg, ...)                                   \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_2(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_4(func, i, arg, ...)                                   \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_3(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_5(func, i, arg, ...)                                   \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_4(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_6(func, i, arg, ...)                                   \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_5(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_7(func, i, arg, ...)                                   \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_6(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_8(func, i, arg, ...)                                   \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_7(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_9(func, i, arg, ...)                                   \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_8(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_10(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_9(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_11(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_10(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_12(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_11(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_13(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_12(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_14(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_13(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_15(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_14(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_16(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_15(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_17(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_16(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_18(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_17(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_19(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_18(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_20(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_19(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_21(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_20(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_22(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_21(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_23(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_22(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_24(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_23(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_25(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_24(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_26(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_25(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_27(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_26(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_28(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_27(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_29(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_28(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_30(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_29(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_31(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_30(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_32(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_31(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_33(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_32(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_34(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_33(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_35(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_34(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_36(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_35(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_37(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_36(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_38(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_37(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_39(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_38(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_40(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_39(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_41(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_40(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_42(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_41(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_43(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_42(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_44(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_43(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_45(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_44(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_46(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_45(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_47(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_46(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_48(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_47(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_49(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_48(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_50(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_49(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_51(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_50(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_52(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_51(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_53(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_52(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_54(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_53(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_55(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_54(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_56(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_55(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_57(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_56(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_58(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_57(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_59(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_58(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_60(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_59(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_61(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_60(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_62(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_61(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_63(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_62(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_64(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_63(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_65(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_64(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_66(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_65(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_67(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_66(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_68(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_67(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_69(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_68(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_70(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_69(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_71(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_70(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_72(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_71(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_73(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_72(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_74(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_73(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_75(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_74(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_76(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_75(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_77(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_76(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_78(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_77(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_79(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_78(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_80(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_79(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_81(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_80(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_82(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_81(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_83(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_82(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_84(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_83(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_85(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_84(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_86(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_85(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_87(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_86(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_88(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_87(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_89(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_88(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_90(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_89(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_91(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_90(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_92(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_91(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_93(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_92(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_94(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_93(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_95(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_94(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_96(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_95(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_97(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_96(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_98(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_97(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_99(func, i, arg, ...)                                  \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_98(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_100(func, i, arg, ...)                                 \
    func(i, arg) MOZI_EXPAND(MOZI_REPEAT_99(func, MOZI_INC(i), __VA_ARGS__))
#define MOZI_REPEAT_101(func, i, arg, ...)                                 \
    func(i, arg)                                                           \
        MOZI_EXPAND(MOZI_REPEAT_100(func, MOZI_INC(i), __VA_ARGS__))

#define MOZI_REPEAT_FIRST_0(func, i, first, arg)
#define MOZI_REPEAT_FIRST_1(func, i, first, arg) func(i, first, arg)
#define MOZI_REPEAT_FIRST_2(func, i, first, arg, ...)                      \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_1(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_3(func, i, first, arg, ...)                      \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_2(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_4(func, i, first, arg, ...)                      \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_3(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_5(func, i, first, arg, ...)                      \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_4(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_6(func, i, first, arg, ...)                      \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_5(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_7(func, i, first, arg, ...)                      \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_6(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_8(func, i, first, arg, ...)                      \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_7(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_9(func, i, first, arg, ...)                      \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_8(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_10(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_9(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_11(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_10(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_12(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_11(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_13(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_12(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_14(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_13(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_15(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_14(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_16(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_15(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_17(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_16(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_18(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_17(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_19(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_18(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_20(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_19(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_21(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_20(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_22(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_21(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_23(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_22(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_24(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_23(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_25(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_24(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_26(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_25(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_27(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_26(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_28(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_27(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_29(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_28(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_30(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_29(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_31(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_30(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_32(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_31(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_33(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_32(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_34(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_33(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_35(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_34(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_36(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_35(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_37(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_36(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_38(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_37(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_39(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_38(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_40(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_39(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_41(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_40(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_42(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_41(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_43(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_42(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_44(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_43(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_45(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_44(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_46(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_45(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_47(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_46(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_48(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_47(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_49(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_48(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_50(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_49(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_51(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_50(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_52(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_51(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_53(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_52(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_54(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_53(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_55(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_54(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_56(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_55(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_57(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_56(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_58(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_57(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_59(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_58(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_60(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_59(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_61(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_60(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_62(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_61(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_63(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_62(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_64(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_63(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_65(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_64(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_66(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_65(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_67(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_66(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_68(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_67(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_69(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_68(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_70(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_69(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_71(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_70(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_72(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_71(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_73(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_72(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_74(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_73(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_75(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_74(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_76(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_75(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_77(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_76(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_78(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_77(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_79(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_78(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_80(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_79(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_81(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_80(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_82(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_81(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_83(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_82(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_84(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_83(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_85(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_84(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_86(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_85(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_87(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_86(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_88(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_87(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_89(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_88(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_90(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_89(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_91(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_90(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_92(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_91(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_93(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_92(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_94(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_93(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_95(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_94(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_96(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_95(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_97(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_96(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_98(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_97(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_99(func, i, first, arg, ...)                     \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_98(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_100(func, i, first, arg, ...)                    \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_99(func, MOZI_INC(i), first, __VA_ARGS__))
#define MOZI_REPEAT_FIRST_101(func, i, first, arg, ...)                    \
    func(i, first, arg) MOZI_EXPAND(                                       \
        MOZI_REPEAT_FIRST_100(func, MOZI_INC(i), first, __VA_ARGS__))

#define MOZI_REPEAT_ON(func, ...)                                          \
    MOZI_EXPAND(MOZI_PASTE(MOZI_REPEAT_, MOZI_GET_ARG_COUNT(__VA_ARGS__))( \
        func, 0, __VA_ARGS__))

#define MOZI_REPEAT_FIRST_ON(func, first, ...)                             \
    MOZI_EXPAND(                                                           \
        MOZI_PASTE(MOZI_REPEAT_FIRST_, MOZI_GET_ARG_COUNT(__VA_ARGS__))(   \
            func, 0, first, __VA_ARGS__))

// MOZI_STRIP((int) x) => MOZI_EAT(int) x => x
#define MOZI_STRIP(x) MOZI_EXPAND(MOZI_EAT x)

#else // Other compilers

#if MOZI_VA_OPT_SUPPORTED // Standardized in C++20
#define MOZI_GET_ARG_COUNT(...)                                            \
    MOZI_INTERNAL_GET_ARG_COUNT_PRIVATE(                                   \
        unused __VA_OPT__(, ) __VA_ARGS__, 102, 101, 100, 99, 98, 97, 96,  \
        95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80,    \
        79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64,    \
        63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48,    \
        47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32,    \
        31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16,    \
        15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#elif defined(__GNUC__) // Extension in GCC/Clang
#define MOZI_GET_ARG_COUNT(...)                                            \
    MOZI_INTERNAL_GET_ARG_COUNT_PRIVATE(                                   \
        unused, ##__VA_ARGS__, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93,  \
        92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77,    \
        76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61,    \
        60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,    \
        44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29,    \
        28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,    \
        12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#else // MOZI_GET_ARG_COUNT() may return 1 here
#define MOZI_GET_ARG_COUNT(...)                                            \
    MOZI_INTERNAL_GET_ARG_COUNT_PRIVATE(                                   \
        unused, __VA_ARGS__, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93,    \
        92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77,    \
        76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61,    \
        60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45,    \
        44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29,    \
        28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13,    \
        12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#endif

#define MOZI_REPEAT_0(func, i, arg)
#define MOZI_REPEAT_1(func, i, arg) \
    func(i, arg)
#define MOZI_REPEAT_2(func, i, arg, ...)                                   \
    func(i, arg) MOZI_REPEAT_1(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_3(func, i, arg, ...)                                   \
    func(i, arg) MOZI_REPEAT_2(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_4(func, i, arg, ...)                                   \
    func(i, arg) MOZI_REPEAT_3(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_5(func, i, arg, ...)                                   \
    func(i, arg) MOZI_REPEAT_4(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_6(func, i, arg, ...)                                   \
    func(i, arg) MOZI_REPEAT_5(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_7(func, i, arg, ...)                                   \
    func(i, arg) MOZI_REPEAT_6(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_8(func, i, arg, ...)                                   \
    func(i, arg) MOZI_REPEAT_7(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_9(func, i, arg, ...)                                   \
    func(i, arg) MOZI_REPEAT_8(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_10(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_9(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_11(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_10(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_12(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_11(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_13(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_12(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_14(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_13(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_15(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_14(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_16(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_15(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_17(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_16(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_18(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_17(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_19(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_18(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_20(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_19(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_21(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_20(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_22(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_21(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_23(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_22(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_24(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_23(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_25(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_24(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_26(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_25(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_27(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_26(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_28(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_27(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_29(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_28(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_30(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_29(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_31(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_30(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_32(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_31(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_33(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_32(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_34(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_33(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_35(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_34(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_36(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_35(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_37(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_36(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_38(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_37(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_39(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_38(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_40(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_39(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_41(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_40(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_42(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_41(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_43(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_42(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_44(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_43(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_45(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_44(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_46(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_45(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_47(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_46(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_48(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_47(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_49(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_48(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_50(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_49(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_51(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_50(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_52(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_51(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_53(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_52(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_54(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_53(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_55(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_54(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_56(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_55(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_57(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_56(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_58(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_57(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_59(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_58(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_60(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_59(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_61(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_60(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_62(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_61(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_63(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_62(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_64(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_63(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_65(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_64(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_66(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_65(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_67(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_66(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_68(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_67(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_69(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_68(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_70(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_69(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_71(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_70(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_72(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_71(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_73(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_72(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_74(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_73(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_75(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_74(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_76(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_75(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_77(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_76(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_78(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_77(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_79(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_78(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_80(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_79(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_81(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_80(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_82(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_81(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_83(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_82(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_84(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_83(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_85(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_84(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_86(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_85(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_87(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_86(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_88(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_87(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_89(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_88(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_90(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_89(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_91(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_90(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_92(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_91(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_93(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_92(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_94(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_93(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_95(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_94(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_96(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_95(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_97(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_96(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_98(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_97(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_99(func, i, arg, ...)                                  \
    func(i, arg) MOZI_REPEAT_98(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_100(func, i, arg, ...)                                 \
    func(i, arg) MOZI_REPEAT_99(func, MOZI_INC(i), __VA_ARGS__)
#define MOZI_REPEAT_101(func, i, arg, ...) func(i, arg) _overflow

#define MOZI_REPEAT_FIRST_0(func, i, first, arg)
#define MOZI_REPEAT_FIRST_1(func, i, first, arg) func(i, first, arg)
#define MOZI_REPEAT_FIRST_2(func, i, first, arg, ...)                      \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_1(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_3(func, i, first, arg, ...)                      \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_2(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_4(func, i, first, arg, ...)                      \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_3(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_5(func, i, first, arg, ...)                      \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_4(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_6(func, i, first, arg, ...)                      \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_5(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_7(func, i, first, arg, ...)                      \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_6(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_8(func, i, first, arg, ...)                      \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_7(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_9(func, i, first, arg, ...)                      \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_8(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_10(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_9(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_11(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_10(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_12(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_11(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_13(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_12(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_14(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_13(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_15(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_14(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_16(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_15(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_17(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_16(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_18(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_17(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_19(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_18(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_20(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_19(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_21(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_20(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_22(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_21(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_23(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_22(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_24(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_23(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_25(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_24(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_26(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_25(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_27(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_26(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_28(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_27(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_29(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_28(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_30(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_29(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_31(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_30(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_32(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_31(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_33(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_32(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_34(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_33(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_35(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_34(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_36(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_35(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_37(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_36(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_38(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_37(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_39(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_38(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_40(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_39(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_41(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_40(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_42(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_41(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_43(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_42(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_44(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_43(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_45(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_44(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_46(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_45(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_47(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_46(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_48(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_47(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_49(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_48(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_50(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_49(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_51(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_50(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_52(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_51(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_53(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_52(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_54(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_53(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_55(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_54(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_56(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_55(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_57(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_56(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_58(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_57(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_59(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_58(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_60(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_59(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_61(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_60(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_62(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_61(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_63(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_62(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_64(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_63(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_65(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_64(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_66(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_65(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_67(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_66(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_68(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_67(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_69(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_68(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_70(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_69(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_71(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_70(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_72(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_71(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_73(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_72(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_74(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_73(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_75(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_74(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_76(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_75(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_77(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_76(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_78(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_77(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_79(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_78(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_80(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_79(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_81(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_80(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_82(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_81(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_83(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_82(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_84(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_83(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_85(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_84(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_86(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_85(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_87(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_86(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_88(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_87(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_89(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_88(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_90(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_89(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_91(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_90(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_92(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_91(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_93(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_92(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_94(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_93(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_95(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_94(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_96(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_95(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_97(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_96(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_98(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_97(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_99(func, i, first, arg, ...)                     \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_98(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_100(func, i, first, arg, ...)                    \
    func(i, first, arg)                                                    \
        MOZI_REPEAT_FIRST_99(func, MOZI_INC(i), first, __VA_ARGS__)
#define MOZI_REPEAT_FIRST_101(func, i, first, arg, ...) _overflow

#define MOZI_REPEAT_ON(func, ...)                                          \
    MOZI_PASTE(MOZI_REPEAT_, MOZI_GET_ARG_COUNT(__VA_ARGS__))              \
    (func, 0, __VA_ARGS__)

#define MOZI_REPEAT_FIRST_ON(func, first, ...)                             \
    MOZI_PASTE(MOZI_REPEAT_FIRST_, MOZI_GET_ARG_COUNT(__VA_ARGS__))        \
    (func, 0, first, __VA_ARGS__)

// MOZI_STRIP((int) x) => MOZI_EAT(int) x => x
#define MOZI_STRIP(x) MOZI_EAT x

#endif

#define MOZI_INC(x) MOZI_PASTE(MOZI_INC_, x)
#define MOZI_INC_0 1
#define MOZI_INC_1 2
#define MOZI_INC_2 3
#define MOZI_INC_3 4
#define MOZI_INC_4 5
#define MOZI_INC_5 6
#define MOZI_INC_6 7
#define MOZI_INC_7 8
#define MOZI_INC_8 9
#define MOZI_INC_9 10
#define MOZI_INC_10 11
#define MOZI_INC_11 12
#define MOZI_INC_12 13
#define MOZI_INC_13 14
#define MOZI_INC_14 15
#define MOZI_INC_15 16
#define MOZI_INC_16 17
#define MOZI_INC_17 18
#define MOZI_INC_18 19
#define MOZI_INC_19 20
#define MOZI_INC_20 21
#define MOZI_INC_21 22
#define MOZI_INC_22 23
#define MOZI_INC_23 24
#define MOZI_INC_24 25
#define MOZI_INC_25 26
#define MOZI_INC_26 27
#define MOZI_INC_27 28
#define MOZI_INC_28 29
#define MOZI_INC_29 30
#define MOZI_INC_30 31
#define MOZI_INC_31 32
#define MOZI_INC_32 33
#define MOZI_INC_33 34
#define MOZI_INC_34 35
#define MOZI_INC_35 36
#define MOZI_INC_36 37
#define MOZI_INC_37 38
#define MOZI_INC_38 39
#define MOZI_INC_39 40
#define MOZI_INC_40 41
#define MOZI_INC_41 42
#define MOZI_INC_42 43
#define MOZI_INC_43 44
#define MOZI_INC_44 45
#define MOZI_INC_45 46
#define MOZI_INC_46 47
#define MOZI_INC_47 48
#define MOZI_INC_48 49
#define MOZI_INC_49 50
#define MOZI_INC_50 51
#define MOZI_INC_51 52
#define MOZI_INC_52 53
#define MOZI_INC_53 54
#define MOZI_INC_54 55
#define MOZI_INC_55 56
#define MOZI_INC_56 57
#define MOZI_INC_57 58
#define MOZI_INC_58 59
#define MOZI_INC_59 60
#define MOZI_INC_60 61
#define MOZI_INC_61 62
#define MOZI_INC_62 63
#define MOZI_INC_63 64
#define MOZI_INC_64 65
#define MOZI_INC_65 66
#define MOZI_INC_66 67
#define MOZI_INC_67 68
#define MOZI_INC_68 69
#define MOZI_INC_69 70
#define MOZI_INC_70 71
#define MOZI_INC_71 72
#define MOZI_INC_72 73
#define MOZI_INC_73 74
#define MOZI_INC_74 75
#define MOZI_INC_75 76
#define MOZI_INC_76 77
#define MOZI_INC_77 78
#define MOZI_INC_78 79
#define MOZI_INC_79 80
#define MOZI_INC_80 81
#define MOZI_INC_81 82
#define MOZI_INC_82 83
#define MOZI_INC_83 84
#define MOZI_INC_84 85
#define MOZI_INC_85 86
#define MOZI_INC_86 87
#define MOZI_INC_87 88
#define MOZI_INC_88 89
#define MOZI_INC_89 90
#define MOZI_INC_90 91
#define MOZI_INC_91 92
#define MOZI_INC_92 93
#define MOZI_INC_93 94
#define MOZI_INC_94 95
#define MOZI_INC_95 96
#define MOZI_INC_96 97
#define MOZI_INC_97 98
#define MOZI_INC_98 99
#define MOZI_INC_99 _overflow

#define MOZI_INTERNAL_GET_ARG_COUNT_PRIVATE(                               \
    e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15,  \
    e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29,  \
    e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43,  \
    e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57,  \
    e58, e59, e60, e61, e62, e63, e64, e65, e66, e67, e68, e69, e70, e71,  \
    e72, e73, e74, e75, e76, e77, e78, e79, e80, e81, e82, e83, e84, e85,  \
    e86, e87, e88, e89, e90, e91, e92, e93, e94, e95, e96, e97, e98, e99,  \
    e100, e101, e102, count, ...)                                          \
    count

#define MOZI_CONCAT(x, y) x##y
#define MOZI_PASTE(x, y) MOZI_CONCAT(x, y)

#define MOZI_STR(x) #x
#define MOZI_STRING(x) MOZI_STR(x)

#define MOZI_EXPAND(...) __VA_ARGS__
#define MOZI_EAT(...)
// MOZI_PAIR((int) x) => MOZI_EXPAND(int) x => int x
#define MOZI_PAIR(x) MOZI_EXPAND x

#define MOZI_DO_PRAGMA(x) _Pragma(#x)

#endif // MOZI_METAMACRO_H
