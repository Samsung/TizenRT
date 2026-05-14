//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <set>

// class set

// set& operator=(initializer_list<value_type> il);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

void basic_test() {
  {
    typedef std::set<int> C;
    typedef C::value_type V;
    C m = {10, 8};
    m = {1, 2, 3, 4, 5, 6};
    TC_ASSERT_EXPR(m.size() == 6);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 6);
    C::const_iterator i = m.cbegin();
    TC_ASSERT_EXPR(*i == V(1));
    TC_ASSERT_EXPR(*++i == V(2));
    TC_ASSERT_EXPR(*++i == V(3));
    TC_ASSERT_EXPR(*++i == V(4));
    TC_ASSERT_EXPR(*++i == V(5));
    TC_ASSERT_EXPR(*++i == V(6));
  }
  {
    typedef std::set<int, std::less<int>, min_allocator<int> > C;
    typedef C::value_type V;
    C m = {10, 8};
    m = {1, 2, 3, 4, 5, 6};
    TC_ASSERT_EXPR(m.size() == 6);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 6);
    C::const_iterator i = m.cbegin();
    TC_ASSERT_EXPR(*i == V(1));
    TC_ASSERT_EXPR(*++i == V(2));
    TC_ASSERT_EXPR(*++i == V(3));
    TC_ASSERT_EXPR(*++i == V(4));
    TC_ASSERT_EXPR(*++i == V(5));
    TC_ASSERT_EXPR(*++i == V(6));
  }
}

void duplicate_keys_test() {
  test_allocator_statistics alloc_stats;
  typedef std::set<int, std::less<int>, test_allocator<int> > Set;
  {
    LIBCPP_ASSERT(alloc_stats.alloc_count == 0);
    Set s({1, 2, 3}, std::less<int>(), test_allocator<int>(&alloc_stats));
    LIBCPP_ASSERT(alloc_stats.alloc_count == 3);
    s = {4, 4, 4, 4, 4};
    LIBCPP_ASSERT(alloc_stats.alloc_count == 1);
    TC_ASSERT_EXPR(s.size() == 1);
    TC_ASSERT_EXPR(*s.begin() == 4);
  }
  LIBCPP_ASSERT(alloc_stats.alloc_count == 0);
}

int tc_containers_associative_set_set_cons_assign_initializer_list(void) {
  basic_test();
  duplicate_keys_test();

  return 0;
}
