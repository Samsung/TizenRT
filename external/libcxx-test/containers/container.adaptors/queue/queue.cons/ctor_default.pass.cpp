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

// explicit queue(Container&& = Container()); // before C++20
// queue() : queue(Container()) {}            // C++20
// explicit queue(Container&&);               // before C++20

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#if TEST_STD_VER >= 11
#include "test_convertible.h"
#include "libcxx_tc_common.h"
#endif

int tc_containers_container_adaptors_queue_queue_cons_ctor_default(void) {
    typedef std::vector<int, limited_allocator<int, 10> > Container;
    typedef std::queue<int, Container> Q;
    Q q;
    TC_ASSERT_EXPR(q.size() == 0);
    q.push(1);
    q.push(2);
    TC_ASSERT_EXPR(q.size() == 2);
    TC_ASSERT_EXPR(q.front() == 1);
    TC_ASSERT_EXPR(q.back() == 2);

#if TEST_STD_VER >= 11
    // It should be explicit, so not convertible before C++20.
    static_assert(test_convertible<Q>(), "");
#endif

    return 0;
}
