//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class set

// set();

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_set_cons_default(void) {
    {
    std::set<int> m;
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    }
#if TEST_STD_VER >= 11
    {
    std::set<int, std::less<int>, min_allocator<int>> m;
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    }
    {
    typedef explicit_allocator<int> A;
        {
        std::set<int, std::less<int>, A> m;
        TC_ASSERT_EXPR(m.empty());
        TC_ASSERT_EXPR(m.begin() == m.end());
        }
        {
        A a;
        std::set<int, std::less<int>, A> m(a);
        TC_ASSERT_EXPR(m.empty());
        TC_ASSERT_EXPR(m.begin() == m.end());
        }
    }
    {
    std::set<int> m = {};
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    }
#endif

  return 0;
}
