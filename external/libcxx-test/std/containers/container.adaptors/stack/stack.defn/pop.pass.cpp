//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <stack>

// void pop();

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_stack_stack_defn_pop(void) {
    std::stack<int> q;
    TC_ASSERT_EXPR(q.size() == 0);
    q.push(1);
    q.push(2);
    q.push(3);
    TC_ASSERT_EXPR(q.size() == 3);
    TC_ASSERT_EXPR(q.top() == 3);
    q.pop();
    TC_ASSERT_EXPR(q.size() == 2);
    TC_ASSERT_EXPR(q.top() == 2);
    q.pop();
    TC_ASSERT_EXPR(q.size() == 1);
    TC_ASSERT_EXPR(q.top() == 1);
    q.pop();
    TC_ASSERT_EXPR(q.size() == 0);

  return 0;
}
