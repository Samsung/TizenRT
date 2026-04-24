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

// explicit queue(const container_type& c);

#include <queue>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#if TEST_STD_VER >= 11
#include "test_convertible.h"
#include "libcxx_tc_common.h"
#endif

template <class C>
C
make(int n)
{
    C c;
    for (int i = 0; i < n; ++i)
        c.push_back(i);
    return c;
}

int tc_containers_container_adaptors_queue_queue_cons_ctor_container(void) {
    typedef std::deque<int> Container;
    typedef std::queue<int> Q;
    Container d = make<Container>(5);
    Q q(d);
    TC_ASSERT_EXPR(q.size() == 5);
    for (std::size_t i = 0; i < d.size(); ++i)
    {
        TC_ASSERT_EXPR(q.front() == d[i]);
        q.pop();
    }

#if TEST_STD_VER >= 11
    static_assert(!test_convertible<Q, const Container&>(), "");
#endif

    return 0;
}
