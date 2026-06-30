//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <queue>

// queue(queue&& q);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"


template <class C>
C
make(int n)
{
    C c;
    for (int i = 0; i < n; ++i)
        c.push_back(MoveOnly(i));
    return c;
}


int tc_containers_container_adaptors_queue_queue_cons_ctor_move(void) {
    std::queue<MoveOnly> q(make<std::deque<MoveOnly> >(5));
    std::queue<MoveOnly> q2 = std::move(q);
    TC_ASSERT_EXPR(q2.size() == 5);
    TC_ASSERT_EXPR(q.empty());

  return 0;
}
