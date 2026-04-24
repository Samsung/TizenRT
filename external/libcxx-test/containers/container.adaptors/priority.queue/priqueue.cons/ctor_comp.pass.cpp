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
// <queue>

// explicit priority_queue(const Compare& comp);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#if TEST_STD_VER >= 11
#include "test_convertible.h"
#include "libcxx_tc_common.h"
#endif

int tc_containers_container_adaptors_priority_queue_priqueue_cons_ctor_comp(void) {
    typedef std::vector<int, limited_allocator<int, 10> > Container;
    typedef std::less<int> Compare;
    typedef std::priority_queue<int, Container> Q;
    Q q((Compare()));
    TC_ASSERT_EXPR(q.size() == 0);
    q.push(1);
    q.push(2);
    TC_ASSERT_EXPR(q.size() == 2);
    TC_ASSERT_EXPR(q.top() == 2);

#if TEST_STD_VER >= 11
    static_assert(!test_convertible<Q, const Compare&>(), "");
#endif

    return 0;
}
