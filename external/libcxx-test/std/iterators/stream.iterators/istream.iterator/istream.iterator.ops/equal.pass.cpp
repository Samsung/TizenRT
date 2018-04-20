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

// class istream_iterator

// template <class T, class charT, class traits, class Distance>
//   bool operator==(const istream_iterator<T,charT,traits,Distance> &x,
//                   const istream_iterator<T,charT,traits,Distance> &y);
//
// template <class T, class charT, class traits, class Distance>
//   bool operator!=(const istream_iterator<T,charT,traits,Distance> &x,
//                   const istream_iterator<T,charT,traits,Distance> &y);

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_istream_iterator_ops_equal(void)
{
    std::istringstream inf1(" 1 23");
    std::istringstream inf2(" 1 23");
    std::istream_iterator<int> i1(inf1);
    std::istream_iterator<int> i2(inf1);
    std::istream_iterator<int> i3(inf2);
    std::istream_iterator<int> i4;
    std::istream_iterator<int> i5;
    TC_ASSERT_EXPR(i1 == i1);
    TC_ASSERT_EXPR(i1 == i2);
    TC_ASSERT_EXPR(i1 != i3);
    TC_ASSERT_EXPR(i1 != i4);
    TC_ASSERT_EXPR(i1 != i5);

    TC_ASSERT_EXPR(i2 == i2);
    TC_ASSERT_EXPR(i2 != i3);
    TC_ASSERT_EXPR(i2 != i4);
    TC_ASSERT_EXPR(i2 != i5);

    TC_ASSERT_EXPR(i3 == i3);
    TC_ASSERT_EXPR(i3 != i4);
    TC_ASSERT_EXPR(i3 != i5);

    TC_ASSERT_EXPR(i4 == i4);
    TC_ASSERT_EXPR(i4 == i5);
    TC_SUCCESS_RESULT();
    return 0;
}
