//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <stack>

// const_reference top() const;

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_stack_stack_defn_top_const(void) {
    std::stack<int> q;
    TC_ASSERT_EXPR(q.size() == 0);
    q.push(1);
    q.push(2);
    q.push(3);
    const std::stack<int>& cqr = q;
    const int& cir = cqr.top();
    TC_ASSERT_EXPR(cir == 3);

  return 0;
}
