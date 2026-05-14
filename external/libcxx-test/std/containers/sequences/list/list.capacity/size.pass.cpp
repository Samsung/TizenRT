//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <list>

// class list

// size_type size() const noexcept;

#include <list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_capacity_size(void) {
    {
    typedef std::list<int> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    TC_ASSERT_EXPR(c.size() == 0);
    c.push_back(C::value_type(2));
    TC_ASSERT_EXPR(c.size() == 1);
    c.push_back(C::value_type(1));
    TC_ASSERT_EXPR(c.size() == 2);
    c.push_back(C::value_type(3));
    TC_ASSERT_EXPR(c.size() == 3);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 2);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 1);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 0);
    }
#if TEST_STD_VER >= 11
    {
    typedef std::list<int, min_allocator<int>> C;
    C c;
    ASSERT_NOEXCEPT(c.size());
    TC_ASSERT_EXPR(c.size() == 0);
    c.push_back(C::value_type(2));
    TC_ASSERT_EXPR(c.size() == 1);
    c.push_back(C::value_type(1));
    TC_ASSERT_EXPR(c.size() == 2);
    c.push_back(C::value_type(3));
    TC_ASSERT_EXPR(c.size() == 3);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 2);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 1);
    c.erase(c.begin());
    TC_ASSERT_EXPR(c.size() == 0);
    }
#endif

  return 0;
}
