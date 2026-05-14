//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// priority_queue();

// size_type size() const;

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_members_size(void) {
    std::priority_queue<int> q;
    TC_ASSERT_EXPR(q.size() == 0);
    q.push(1);
    TC_ASSERT_EXPR(q.size() == 1);
    q.pop();
    TC_ASSERT_EXPR(q.size() == 0);

  return 0;
}
