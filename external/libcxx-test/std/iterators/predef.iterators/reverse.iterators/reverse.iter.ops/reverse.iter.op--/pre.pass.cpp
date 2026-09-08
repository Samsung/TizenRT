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

// constexpr reverse_iterator& operator--();
//
// constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
static int
test(It i, It x)
{
    std::reverse_iterator<It> r(i);
    std::reverse_iterator<It>& rr = --r;
    TC_ASSERT_EXPR(r.base() == x);
    TC_ASSERT_EXPR(&rr == &r);
    return 0;
}

int tc_libcxx_iterators_reverse_iter_op___pre(void)
{
    const char* s = "123";
    TC_ASSERT_FUNC((test(bidirectional_iterator<const char*>(s+1), bidirectional_iterator<const char*>(s+2))));
    TC_ASSERT_FUNC((test(random_access_iterator<const char*>(s+1), random_access_iterator<const char*>(s+2))));
    TC_ASSERT_FUNC((test(s+1, s+2)));

#if TEST_STD_VER > 14
    {
        constexpr const char *p = "123456789";
        typedef std::reverse_iterator<const char *> RI;
        constexpr RI it1 = std::make_reverse_iterator(p);
        constexpr RI it2 = std::make_reverse_iterator(p+1);
        static_assert(it1 != it2, "");
        constexpr RI it3 = -- std::make_reverse_iterator(p);
        static_assert(it1 != it3, "");
        static_assert(it2 == it3, "");
        static_assert(*(--std::make_reverse_iterator(p)) == '1', "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
