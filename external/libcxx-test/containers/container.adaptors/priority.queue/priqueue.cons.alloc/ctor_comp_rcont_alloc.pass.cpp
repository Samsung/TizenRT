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

// template <class Alloc>
//     priority_queue(const Compare& comp, container_type&& c,
//                    const Alloc& a);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
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

template <class T>
struct test
    : public std::priority_queue<T, std::vector<T, test_allocator<T> > >
{
    typedef std::priority_queue<T, std::vector<T, test_allocator<T> > > base;
    typedef typename base::container_type container_type;
    typedef typename base::value_compare value_compare;

    explicit test(const test_allocator<int>& a) : base(a) {}
    test(const value_compare& compare, const test_allocator<int>& a)
        : base(compare, a) {}
    test(const value_compare& compare, const container_type& container,
        const test_allocator<int>& a) : base(compare, container, a) {}
#if TEST_STD_VER >= 11 // testing rvalue ctor
    test(const value_compare& compare, container_type&& container,
         const test_allocator<int>& a) : base(compare, std::move(container), a) {}
    test(test&& q, const test_allocator<int>& a) : base(std::move(q), a) {}
#endif
    test_allocator<int> get_allocator() {return c.get_allocator();}

    using base::c;
};

int tc_containers_container_adaptors_priority_queue_priqueue_cons_alloc_ctor_comp_rcont_alloc(void) {
    typedef std::vector<int, test_allocator<int> > C;
    test<int> q(std::less<int>(), make<C>(5), test_allocator<int>(3));
    TC_ASSERT_EXPR(q.c.get_allocator() == test_allocator<int>(3));
    TC_ASSERT_EXPR(q.size() == 5);
    TC_ASSERT_EXPR(q.top() == 4);

  return 0;
}
