//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <stack>

// void swap(stack& q);

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
        c.push(i);
    return c;
}

int tc_containers_container_adaptors_stack_stack_defn_swap(void) {
    std::stack<int> q1 = make<std::stack<int> >(5);
    std::stack<int> q2 = make<std::stack<int> >(10);
    std::stack<int> q1_save = q1;
    std::stack<int> q2_save = q2;
    q1.swap(q2);
    TC_ASSERT_EXPR(q1 == q2_save);
    TC_ASSERT_EXPR(q2 == q1_save);

  return 0;
}
