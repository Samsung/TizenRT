//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class set

// size_type size() const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_size(void) {
    {
    typedef std::set<int> M;
    M m;
    TC_ASSERT_EXPR(m.size() == 0);
    m.insert(M::value_type(2));
    TC_ASSERT_EXPR(m.size() == 1);
    m.insert(M::value_type(1));
    TC_ASSERT_EXPR(m.size() == 2);
    m.insert(M::value_type(3));
    TC_ASSERT_EXPR(m.size() == 3);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 0);
    }
#if TEST_STD_VER >= 11
    {
    typedef std::set<int, std::less<int>, min_allocator<int>> M;
    M m;
    TC_ASSERT_EXPR(m.size() == 0);
    m.insert(M::value_type(2));
    TC_ASSERT_EXPR(m.size() == 1);
    m.insert(M::value_type(1));
    TC_ASSERT_EXPR(m.size() == 2);
    m.insert(M::value_type(3));
    TC_ASSERT_EXPR(m.size() == 3);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 0);
    }
#endif

  return 0;
}
