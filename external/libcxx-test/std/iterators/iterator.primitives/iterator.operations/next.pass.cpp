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

// template <InputIterator Iter>
//   Iter next(Iter x, Iter::difference_type n = 1);

// LWG #2353 relaxed the requirement on next from ForwardIterator to InputIterator

#include <iterator>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "test_iterators.h"

template <class It>
static int
test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    TC_ASSERT_EXPR(std::next(i, n) == x);
    return 0;
}

template <class It>
static int
test(It i, It x)
{
    TC_ASSERT_EXPR(std::next(i) == x);
    return 0;
}

#if TEST_STD_VER > 14
template <class It>
constexpr bool
constexpr_test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    return std::next(i, n) == x;
    return 0;
}

template <class It>
constexpr bool
constexpr_test(It i, It x)
{
    return std::next(i) == x;
    return 0;
}
#endif

int tc_libcxx_iterators_iterator_operations_next(void)
{
    {
    const char* s = "1234567890";
    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), 10, input_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(forward_iterator<const char*>(s), 10, forward_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(bidirectional_iterator<const char*>(s), 10, bidirectional_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s), 10, random_access_iterator<const char*>(s+10))));
    TC_ASSERT_FUNC((test(s, 10, s+10)));

    TC_ASSERT_FUNC((test(input_iterator<const char*>(s), input_iterator<const char*>(s+1))));
    TC_ASSERT_FUNC((test(forward_iterator<const char*>(s), forward_iterator<const char*>(s+1))));
    TC_ASSERT_FUNC((test(bidirectional_iterator<const char*>(s), bidirectional_iterator<const char*>(s+1))));
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s), random_access_iterator<const char*>(s+1))));
    TC_ASSERT_FUNC((test(s, s+1)));
    }
#if TEST_STD_VER > 14
    {
    constexpr const char* s = "1234567890";
    static_assert( constexpr_test(input_iterator<const char*>(s), 10, input_iterator<const char*>(s+10)), "" );
    static_assert( constexpr_test(forward_iterator<const char*>(s), 10, forward_iterator<const char*>(s+10)), "" );
    static_assert( constexpr_test(bidirectional_iterator<const char*>(s), 10, bidirectional_iterator<const char*>(s+10)), "" );
    static_assert( constexpr_test(random_access_iterator<const char*>(s), 10, random_access_iterator<const char*>(s+10)), "" );
    static_assert( constexpr_test(s, 10, s+10), "" );

    static_assert( constexpr_test(input_iterator<const char*>(s), input_iterator<const char*>(s+1)), "" );
    static_assert( constexpr_test(forward_iterator<const char*>(s), forward_iterator<const char*>(s+1)), "" );
    static_assert( constexpr_test(bidirectional_iterator<const char*>(s), bidirectional_iterator<const char*>(s+1)), "" );
    static_assert( constexpr_test(random_access_iterator<const char*>(s), random_access_iterator<const char*>(s+1)), "" );
    static_assert( constexpr_test(s, s+1), "" );
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
