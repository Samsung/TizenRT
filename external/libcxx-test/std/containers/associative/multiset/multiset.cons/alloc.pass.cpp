//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class multiset

// multiset(const allocator_type& a);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_alloc(void) {
    typedef std::less<int> C;
    typedef test_allocator<int> A;
    std::multiset<int, C, A> m(A(5));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.get_allocator() == A(5));

  return 0;
}
