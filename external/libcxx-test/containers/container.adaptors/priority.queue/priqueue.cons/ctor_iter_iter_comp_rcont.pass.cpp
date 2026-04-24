/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
// UNSUPPORTED: c++03

// <queue>

// template <class InputIterator>
//   priority_queue(InputIterator first, InputIterator last,
//                  const Compare& comp, container_type&& c);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_cons_ctor_iter_iter_comp_rcont(void) {
    int a[] = {3, 5, 2, 0, 6, 8, 1};
    const int n = sizeof(a)/sizeof(a[0]);
    std::priority_queue<MoveOnly> q(a+n/2, a+n,
                                    std::less<MoveOnly>(),
                                    std::vector<MoveOnly>(a, a+n/2));
    TC_ASSERT_EXPR(q.size() == n);
    TC_ASSERT_EXPR(q.top() == MoveOnly(8));

  return 0;
}
