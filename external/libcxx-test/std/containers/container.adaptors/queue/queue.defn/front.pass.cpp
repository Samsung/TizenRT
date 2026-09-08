//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// reference front();

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_queue_queue_defn_front(void) {
    std::queue<int> q;
    TC_ASSERT_EXPR(q.size() == 0);
    q.push(1);
    q.push(2);
    q.push(3);
    int& ir = q.front();
    TC_ASSERT_EXPR(ir == 1);

  return 0;
}
