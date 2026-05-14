//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <stack>

// void push(const value_type& v);

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_stack_stack_defn_push(void) {
    std::stack<int> q;
    q.push(1);
    TC_ASSERT_EXPR(q.size() == 1);
    TC_ASSERT_EXPR(q.top() == 1);
    q.push(2);
    TC_ASSERT_EXPR(q.size() == 2);
    TC_ASSERT_EXPR(q.top() == 2);
    q.push(3);
    TC_ASSERT_EXPR(q.size() == 3);
    TC_ASSERT_EXPR(q.top() == 3);

  return 0;
}
