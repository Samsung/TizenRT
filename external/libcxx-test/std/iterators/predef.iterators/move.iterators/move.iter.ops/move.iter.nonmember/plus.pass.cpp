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

// move_iterator

// template <RandomAccessIterator Iter>
//   move_iterator<Iter>
//   operator+(Iter::difference_type n, const move_iterator<Iter>& x);
//
//  constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
static int
test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    const std::move_iterator<It> r(i);
    std::move_iterator<It> rr = n + r;
    TC_ASSERT_EXPR(rr.base() == x);
    return 0;
}

int tc_libcxx_iterators_move_iter_nonmember_plus(void)
{
    char s[] = "1234567890";
    TC_ASSERT_FUNC((test(random_access_iterator<char*>(s+5), 5, random_access_iterator<char*>(s+10))));
    TC_ASSERT_FUNC((test(s+5, 5, s+10)));

#if TEST_STD_VER > 14
    {
    constexpr const char *p = "123456789";
    typedef std::move_iterator<const char *> MI;
    constexpr MI it1 = std::make_move_iterator(p);
    constexpr MI it2 = std::make_move_iterator(p + 5);
    constexpr MI it3 = it1 + 5;
    static_assert(it1 != it2, "");
    static_assert(it1 != it3, "");
    static_assert(it2 == it3, "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
