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

// reverse_iterator

// template <RandomAccessIterator Iter1, RandomAccessIterator Iter2>
//   requires HasGreater<Iter1, Iter2>
//   constexpr bool
//   operator>(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y);
//
//   constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
static int
test(It l, It r, bool x)
{
    const std::reverse_iterator<It> r1(l);
    const std::reverse_iterator<It> r2(r);
    TC_ASSERT_EXPR((r1 > r2) == x);
    return 0;
}

int tc_libcxx_iterators_reverse_iter_opgt_test(void)
{
    const char* s = "1234567890";
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s), random_access_iterator<const char*>(s), false)));
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s), random_access_iterator<const char*>(s+1), true)));
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s+1), random_access_iterator<const char*>(s), false)));
    TC_ASSERT_FUNC((test(s, s, false)));
    TC_ASSERT_FUNC((test(s, s+1, true)));
    TC_ASSERT_FUNC((test(s+1, s, false)));

#if TEST_STD_VER > 14
    {
        constexpr const char *p = "123456789";
        typedef std::reverse_iterator<const char *> RI;
        constexpr RI it1 = std::make_reverse_iterator(p);
        constexpr RI it2 = std::make_reverse_iterator(p);
        constexpr RI it3 = std::make_reverse_iterator(p+1);
        static_assert(!(it1 > it2), "");
        static_assert( (it1 > it3), "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
