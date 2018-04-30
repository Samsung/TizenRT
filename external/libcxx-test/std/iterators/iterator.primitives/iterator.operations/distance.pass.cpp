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

// template <InputIterator Iter>
//   Iter::difference_type
//   distance(Iter first, Iter last);
//
// template <RandomAccessIterator Iter>
//   Iter::difference_type
//   distance(Iter first, Iter last);

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_iterators.h"

template <class It>
static int
test(It first, It last, typename std::iterator_traits<It>::difference_type x)
{
    TC_ASSERT_EXPR(std::distance(first, last) == x);
    return 0;
}

#if TEST_STD_VER > 14
template <class It>
constexpr bool
constexpr_test(It first, It last, typename std::iterator_traits<It>::difference_type x)
{
    return std::distance(first, last) == x;
    return 0;
}
#endif

int tc_libcxx_iterators_iterator_operations_distance(void)
{
    {
    const char* s = "1234567890";
    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s+10), 10)));
    TC_ASSERT_FUNC((test(forward_iterator<const char*>(s), forward_iterator<const char*>(s+10), 10)));
    TC_ASSERT_FUNC((test(bidirectional_iterator<const char*>(s), bidirectional_iterator<const char*>(s+10), 10)));
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s), random_access_iterator<const char*>(s+10), 10)));
    TC_ASSERT_FUNC((test(s, s+10, 10)));
    }
#if TEST_STD_VER > 14
    {
    constexpr const char* s = "1234567890";
    static_assert( constexpr_test(input_iterator<const char*>(s), input_iterator<const char*>(s+10), 10), "");
    static_assert( constexpr_test(forward_iterator<const char*>(s), forward_iterator<const char*>(s+10), 10), "");
    static_assert( constexpr_test(bidirectional_iterator<const char*>(s), bidirectional_iterator<const char*>(s+10), 10), "");
    static_assert( constexpr_test(random_access_iterator<const char*>(s), random_access_iterator<const char*>(s+10), 10), "");
    static_assert( constexpr_test(s, s+10, 10), "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
