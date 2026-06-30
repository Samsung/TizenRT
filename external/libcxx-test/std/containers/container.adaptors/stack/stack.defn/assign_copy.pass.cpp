//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <stack>

// stack& operator=(const stack& q);

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class C>
C
make(int n)
{
    C c;
    for (int i = 0; i < n; ++i)
        c.push_back(i);
    return c;
}

int tc_containers_container_adaptors_stack_stack_defn_assign_copy(void) {
    std::stack<int> q(make<std::deque<int> >(5));
    std::stack<int> q2;
    q2 = q;
    TC_ASSERT_EXPR(q2 == q);

  return 0;
}
