//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <queue>

// void push(value_type&& v);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_queue_queue_defn_push_rv(void) {
    std::queue<MoveOnly> q;
    q.push(MoveOnly(1));
    TC_ASSERT_EXPR(q.size() == 1);
    TC_ASSERT_EXPR(q.front() == MoveOnly(1));
    TC_ASSERT_EXPR(q.back() == MoveOnly(1));
    q.push(MoveOnly(2));
    TC_ASSERT_EXPR(q.size() == 2);
    TC_ASSERT_EXPR(q.front() == MoveOnly(1));
    TC_ASSERT_EXPR(q.back() == MoveOnly(2));
    q.push(MoveOnly(3));
    TC_ASSERT_EXPR(q.size() == 3);
    TC_ASSERT_EXPR(q.front() == MoveOnly(1));
    TC_ASSERT_EXPR(q.back() == MoveOnly(3));

  return 0;
}
