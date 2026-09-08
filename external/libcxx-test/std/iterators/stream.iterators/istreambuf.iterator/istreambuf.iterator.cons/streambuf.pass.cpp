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

// <iterator>

// istreambuf_iterator

// istreambuf_iterator(basic_streambuf<charT,traits>* s) throw();

#include <iterator>
#include <sstream>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_istreambuf_iterator_cons_streambuf(void)
{
    {
        std::istreambuf_iterator<char> i(nullptr);
        TC_ASSERT_EXPR(i == std::istreambuf_iterator<char>());
    }
    {
        std::istringstream inf;
        std::istreambuf_iterator<char> i(inf.rdbuf());
        TC_ASSERT_EXPR(i == std::istreambuf_iterator<char>());
    }
    {
        std::istringstream inf("a");
        std::istreambuf_iterator<char> i(inf.rdbuf());
        TC_ASSERT_EXPR(i != std::istreambuf_iterator<char>());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::istreambuf_iterator<wchar_t> i(nullptr);
        TC_ASSERT_EXPR(i == std::istreambuf_iterator<wchar_t>());
    }
    {
        std::wistringstream inf;
        std::istreambuf_iterator<wchar_t> i(inf.rdbuf());
        TC_ASSERT_EXPR(i == std::istreambuf_iterator<wchar_t>());
    }
    {
        std::wistringstream inf(L"a");
        std::istreambuf_iterator<wchar_t> i(inf.rdbuf());
        TC_ASSERT_EXPR(i != std::istreambuf_iterator<wchar_t>());
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
