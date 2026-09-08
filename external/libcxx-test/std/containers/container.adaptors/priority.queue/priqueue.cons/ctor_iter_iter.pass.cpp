//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// template <class InputIterator>
//   priority_queue(InputIterator first, InputIterator last);

#include <queue>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_cons_ctor_iter_iter(void) {
    int a[] = {3, 5, 2, 0, 6, 8, 1};
    int* an = a + sizeof(a)/sizeof(a[0]);
    std::priority_queue<int> q(a, an);
    TC_ASSERT_EXPR(q.size() == static_cast<std::size_t>(an - a));
    TC_ASSERT_EXPR(q.top() == 8);

  return 0;
}
