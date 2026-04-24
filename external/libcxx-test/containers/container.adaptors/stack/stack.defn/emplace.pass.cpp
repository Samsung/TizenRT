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

// template <class... Args> decltype(auto) emplace(Args&&... args);
// return type is 'decltype(auto)' in C++17; 'void' before
//  whatever the return type of the underlying container's emplace_back() returns.

#include <stack>
#include <cassert>
#include <vector>

#include "test_macros.h"

#include "../../../Emplaceable.h"
#include "libcxx_tc_common.h"

template <typename Stack>
void test_return_type() {
    typedef typename Stack::container_type Container;
    typedef typename Container::value_type value_type;
    typedef decltype(std::declval<Stack>().emplace(std::declval<value_type &>()))     stack_return_type;

#if TEST_STD_VER > 14
    typedef decltype(std::declval<Container>().emplace_back(std::declval<value_type>())) container_return_type;
    static_assert(std::is_same<stack_return_type, container_return_type>::value, "");
#else
    static_assert(std::is_same<stack_return_type, void>::value, "");
#endif
}

int tc_containers_container_adaptors_stack_stack_defn_emplace(void) {
    test_return_type<std::stack<int> > ();
    test_return_type<std::stack<int, std::vector<int> > > ();

    std::stack<Emplaceable> q;
#if TEST_STD_VER > 14
    typedef Emplaceable T;
    T& r1 = q.emplace(1, 2.5);
    TC_ASSERT_EXPR(&r1 == &q.top());
    T& r2 = q.emplace(2, 3.5);
    TC_ASSERT_EXPR(&r2 == &q.top());
    T& r3 = q.emplace(3, 4.5);
    TC_ASSERT_EXPR(&r3 == &q.top());
#else
    q.emplace(1, 2.5);
    q.emplace(2, 3.5);
    q.emplace(3, 4.5);
#endif
    TC_ASSERT_EXPR(q.size() == 3);
    TC_ASSERT_EXPR(q.top() == Emplaceable(3, 4.5));

  return 0;
}
