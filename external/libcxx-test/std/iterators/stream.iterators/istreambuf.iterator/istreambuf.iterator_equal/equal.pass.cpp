/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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
    TC_SUCCESS_RESULT();
    return 0;
}
