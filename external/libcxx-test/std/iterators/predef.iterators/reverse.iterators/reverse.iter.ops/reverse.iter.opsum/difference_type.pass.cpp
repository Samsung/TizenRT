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

// reverse_iterator

// template <RandomAccessIterator Iterator>
//   constexpr reverse_iterator<Iter>
//   operator+(Iter::difference_type n, const reverse_iterator<Iter>& x);
//
// constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
static int
test(It i, typename std::iterator_traits<It>::difference_type n, It x)
{
    const std::reverse_iterator<It> r(i);
    std::reverse_iterator<It> rr = n + r;
    TC_ASSERT_EXPR(rr.base() == x);
    return 0;
}

int tc_libcxx_iterators_reverse_iter_opsum_difference_type(void)
{
    const char* s = "1234567890";
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s+5), 5, random_access_iterator<const char*>(s))));
    TC_ASSERT_FUNC((test(s+5, 5, s)));

#if TEST_STD_VER > 14
    {
        constexpr const char *p = "123456789";
        typedef std::reverse_iterator<const char *> RI;
        constexpr RI it1 = std::make_reverse_iterator(p);
        constexpr RI it2 = std::make_reverse_iterator(p + 5);
        constexpr RI it3 = 5 + it2;
        static_assert(it1 != it2, "");
        static_assert(it1 == it3, "");
        static_assert(it2 != it3, "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
