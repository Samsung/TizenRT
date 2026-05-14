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

// set(initializer_list<value_type> il, const key_compare& comp = key_compare());

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_set_cons_initializer_list(void) {
    {
    typedef std::set<int> C;
    typedef C::value_type V;
    C m = {1, 2, 3, 4, 5, 6};
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
    typedef std::set<int, std::less<int>, min_allocator<int>> C;
    typedef C::value_type V;
    C m = {1, 2, 3, 4, 5, 6};
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
