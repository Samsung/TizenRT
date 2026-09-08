//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <cctype>

#include <cctype>
#include <type_traits>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#ifdef isalnum
#error isalnum defined
#endif

#ifdef isalpha
#error isalpha defined
#endif

#ifdef isblank
#error isblank defined
#endif

#ifdef iscntrl
#error iscntrl defined
#endif

#ifdef isdigit
#error isdigit defined
#endif

#ifdef isgraph
#error isgraph defined
#endif

#ifdef islower
#error islower defined
#endif

#ifdef isprint
#error isprint defined
#endif

#ifdef ispunct
#error ispunct defined
#endif

#ifdef isspace
#error isspace defined
#endif

#ifdef isupper
#error isupper defined
#endif

#ifdef isxdigit
#error isxdigit defined
#endif

#ifdef tolower
#error tolower defined
#endif

#ifdef toupper
#error toupper defined
#endif

int tc_libcxx_strings_c_strings_cctype(void)
{
    static_assert((std::is_same<decltype(std::isalnum(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::isalpha(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::isblank(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::iscntrl(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::isdigit(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::isgraph(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::islower(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::isprint(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::ispunct(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::isspace(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::isupper(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::isxdigit(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::tolower(0)), int>::value), "");
    static_assert((std::is_same<decltype(std::toupper(0)), int>::value), "");

    TC_ASSERT_EXPR(std::isalnum('a'));
    TC_ASSERT_EXPR(std::isalpha('a'));
    TC_ASSERT_EXPR(std::isblank(' '));
    TC_ASSERT_EXPR(!std::iscntrl(' '));
    TC_ASSERT_EXPR(!std::isdigit('a'));
    TC_ASSERT_EXPR(std::isgraph('a'));
    TC_ASSERT_EXPR(std::islower('a'));
    TC_ASSERT_EXPR(std::isprint('a'));
    TC_ASSERT_EXPR(!std::ispunct('a'));
    TC_ASSERT_EXPR(!std::isspace('a'));
    TC_ASSERT_EXPR(!std::isupper('a'));
    TC_ASSERT_EXPR(std::isxdigit('a'));
    TC_ASSERT_EXPR(std::tolower('A') == 'a');
    TC_ASSERT_EXPR(std::toupper('a') == 'A');
    TC_SUCCESS_RESULT();
    return 0;
}
