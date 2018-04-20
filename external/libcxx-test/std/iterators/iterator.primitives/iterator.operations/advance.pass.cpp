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

//   All of these became constexpr in C++17
//
// template <InputIterator Iter>
//   constexpr static int advance(Iter& i, Iter::difference_type n);
//
// template <BidirectionalIterator Iter>
//   constexpr static int advance(Iter& i, Iter::difference_type n);
//
// template <RandomAccessIterator Iter>
//   constexpr static int advance(Iter& i, Iter::difference_type n);

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_iterators.h"

template <class It>
static int
test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    std::advance(i, n);
    TC_ASSERT_EXPR(i == x);
    return 0;
}

#if TEST_STD_VER > 14
template <class It>
constexpr bool
constepxr_test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    std::advance(i, n);
    return i == x;
    return 0;
}
#endif

int tc_libcxx_iterators_iterator_operations_advance(void)
{
    {
    const char* s = "1234567890";
    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), 10, input_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(forward_iterator<const char*>(s), 10, forward_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(bidirectional_iterator<const char*>(s+5), 5, bidirectional_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(bidirectional_iterator<const char*>(s+5), -5, bidirectional_iterator<const char*>(s))));
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s+5), 5, random_access_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s+5), -5, random_access_iterator<const char*>(s))));
    TC_ASSERT_FUNC((test(s+5, 5, s+10)));
    TC_ASSERT_FUNC((test(s+5, -5, s)));
    }
#if TEST_STD_VER > 14
    {
    constexpr const char* s = "1234567890";
    static_assert( constepxr_test(input_iterator<const char*>(s), 10, input_iterator<const char*>(s+10)), "" );
    static_assert( constepxr_test(forward_iterator<const char*>(s), 10, forward_iterator<const char*>(s+10)), "" );
    static_assert( constepxr_test(bidirectional_iterator<const char*>(s+5), 5, bidirectional_iterator<const char*>(s+10)), "" );
    static_assert( constepxr_test(bidirectional_iterator<const char*>(s+5), -5, bidirectional_iterator<const char*>(s)), "" );
    static_assert( constepxr_test(random_access_iterator<const char*>(s+5), 5, random_access_iterator<const char*>(s+10)), "" );
    static_assert( constepxr_test(random_access_iterator<const char*>(s+5), -5, random_access_iterator<const char*>(s)), "" );
    static_assert( constepxr_test(s+5, 5, s+10), "" );
    static_assert( constepxr_test(s+5, -5, s), "" );
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
