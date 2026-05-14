//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <set>

// class multiset

// multiset& operator=(initializer_list<value_type> il);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_assign_initializer_list(void) {
    {
    typedef std::multiset<int> C;
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
    typedef std::multiset<int, std::less<int>, min_allocator<int>> C;
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

  return 0;
}
