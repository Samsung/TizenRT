//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// priority_queue(const priority_queue&) = default;

#include <queue>
#include <cassert>
#include <functional>

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

int tc_containers_container_adaptors_priority_queue_priqueue_cons_ctor_copy(void) {
    std::vector<int> v = make<std::vector<int> >(5);
    std::priority_queue<int, std::vector<int>, std::greater<int> > qo(std::greater<int>(), v);
    std::priority_queue<int, std::vector<int>, std::greater<int> > q = qo;
    TC_ASSERT_EXPR(q.size() == 5);
    TC_ASSERT_EXPR(q.top() == 0);

  return 0;
}
