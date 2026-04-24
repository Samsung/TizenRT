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

// <stack>

// explicit stack(Container&&= Container());  // before C++20
// stack() : stack(Container()) {}            // C++20
// explicit stack(Container&&);               // C++20

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
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
        c.push_back(MoveOnly(i));
    return c;
}

int tc_containers_container_adaptors_stack_stack_cons_ctor_rcontainer(void) {
    typedef std::deque<MoveOnly> Container;
    typedef std::stack<MoveOnly> Q;
    Q q(make<Container>(5));
    TC_ASSERT_EXPR(q.size() == 5);

#if TEST_STD_VER >= 11
    static_assert(!test_convertible<Q, Container&&>(), "");
#endif

  return 0;
}
