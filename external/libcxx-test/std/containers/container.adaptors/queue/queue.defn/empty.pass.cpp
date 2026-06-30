//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// bool empty() const;

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_queue_queue_defn_empty(void) {
    std::queue<int> q;
    TC_ASSERT_EXPR(q.empty());
    q.push(1);
    TC_ASSERT_EXPR(!q.empty());
    q.pop();
    TC_ASSERT_EXPR(q.empty());

  return 0;
}
