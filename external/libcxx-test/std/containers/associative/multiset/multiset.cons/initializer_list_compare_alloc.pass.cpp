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

// multiset(initializer_list<value_type> il, const key_compare& comp, const allocator_type& a);

#include <set>
#include <cassert>
#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_initializer_list_compare_alloc(void) {
    typedef test_less<int> Cmp;
    typedef test_allocator<int> A;
    typedef std::multiset<int, Cmp, A> C;
    typedef C::value_type V;
    C m({1, 2, 3, 4, 5, 6}, Cmp(10), A(4));
    TC_ASSERT_EXPR(m.size() == 6);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 6);
    C::const_iterator i = m.cbegin();
    TC_ASSERT_EXPR(*i == V(1));
    TC_ASSERT_EXPR(*++i == V(2));
    TC_ASSERT_EXPR(*++i == V(3));
    TC_ASSERT_EXPR(*++i == V(4));
    TC_ASSERT_EXPR(*++i == V(5));
    TC_ASSERT_EXPR(*++i == V(6));
    TC_ASSERT_EXPR(m.key_comp() == Cmp(10));
    TC_ASSERT_EXPR(m.get_allocator() == A(4));

  return 0;
}
