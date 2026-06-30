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

// bool equal(istreambuf_iterator<charT,traits>& b) const;

#include <iterator>
#include <sstream>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_istreambuf_iterator_equal_equal(void)
{
    {
        std::istringstream inf1("abc");
        std::istringstream inf2("def");
        std::istreambuf_iterator<char> i1(inf1);
        std::istreambuf_iterator<char> i2(inf2);
        std::istreambuf_iterator<char> i3;
        std::istreambuf_iterator<char> i4;
        std::istreambuf_iterator<char> i5(nullptr);

        TC_ASSERT_EXPR( i1.equal(i1));
        TC_ASSERT_EXPR( i1.equal(i2));
        TC_ASSERT_EXPR(!i1.equal(i3));
        TC_ASSERT_EXPR(!i1.equal(i4));
        TC_ASSERT_EXPR(!i1.equal(i5));

        TC_ASSERT_EXPR( i2.equal(i1));
        TC_ASSERT_EXPR( i2.equal(i2));
        TC_ASSERT_EXPR(!i2.equal(i3));
        TC_ASSERT_EXPR(!i2.equal(i4));
        TC_ASSERT_EXPR(!i2.equal(i5));

        TC_ASSERT_EXPR(!i3.equal(i1));
        TC_ASSERT_EXPR(!i3.equal(i2));
        TC_ASSERT_EXPR( i3.equal(i3));
        TC_ASSERT_EXPR( i3.equal(i4));
        TC_ASSERT_EXPR( i3.equal(i5));

        TC_ASSERT_EXPR(!i4.equal(i1));
        TC_ASSERT_EXPR(!i4.equal(i2));
        TC_ASSERT_EXPR( i4.equal(i3));
        TC_ASSERT_EXPR( i4.equal(i4));
        TC_ASSERT_EXPR( i4.equal(i5));

        TC_ASSERT_EXPR(!i5.equal(i1));
        TC_ASSERT_EXPR(!i5.equal(i2));
        TC_ASSERT_EXPR( i5.equal(i3));
        TC_ASSERT_EXPR( i5.equal(i4));
        TC_ASSERT_EXPR( i5.equal(i5));
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::wistringstream inf1(L"abc");
        std::wistringstream inf2(L"def");
        std::istreambuf_iterator<wchar_t> i1(inf1);
        std::istreambuf_iterator<wchar_t> i2(inf2);
        std::istreambuf_iterator<wchar_t> i3;
        std::istreambuf_iterator<wchar_t> i4;
        std::istreambuf_iterator<wchar_t> i5(nullptr);

        TC_ASSERT_EXPR( i1.equal(i1));
        TC_ASSERT_EXPR( i1.equal(i2));
        TC_ASSERT_EXPR(!i1.equal(i3));
        TC_ASSERT_EXPR(!i1.equal(i4));
        TC_ASSERT_EXPR(!i1.equal(i5));

        TC_ASSERT_EXPR( i2.equal(i1));
        TC_ASSERT_EXPR( i2.equal(i2));
        TC_ASSERT_EXPR(!i2.equal(i3));
        TC_ASSERT_EXPR(!i2.equal(i4));
        TC_ASSERT_EXPR(!i2.equal(i5));

        TC_ASSERT_EXPR(!i3.equal(i1));
        TC_ASSERT_EXPR(!i3.equal(i2));
        TC_ASSERT_EXPR( i3.equal(i3));
        TC_ASSERT_EXPR( i3.equal(i4));
        TC_ASSERT_EXPR( i3.equal(i5));

        TC_ASSERT_EXPR(!i4.equal(i1));
        TC_ASSERT_EXPR(!i4.equal(i2));
        TC_ASSERT_EXPR( i4.equal(i3));
        TC_ASSERT_EXPR( i4.equal(i4));
        TC_ASSERT_EXPR( i4.equal(i5));

        TC_ASSERT_EXPR(!i5.equal(i1));
        TC_ASSERT_EXPR(!i5.equal(i2));
        TC_ASSERT_EXPR( i5.equal(i3));
        TC_ASSERT_EXPR( i5.equal(i4));
        TC_ASSERT_EXPR( i5.equal(i5));
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
