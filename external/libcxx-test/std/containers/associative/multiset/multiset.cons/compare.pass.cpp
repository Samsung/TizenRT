//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class multiset

// explicit multiset(const key_compare& comp);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_compare(void) {
    typedef test_less<int> C;
    const std::multiset<int, C> m(C(3));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    TC_ASSERT_EXPR(m.value_comp() == C(3));

  return 0;
}
