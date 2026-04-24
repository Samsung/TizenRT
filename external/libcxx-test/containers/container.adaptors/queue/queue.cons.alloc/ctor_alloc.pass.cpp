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
//   explicit queue(const Alloc& a);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

struct test
    : private std::queue<int, std::deque<int, test_allocator<int> > >
{
    typedef std::queue<int, std::deque<int, test_allocator<int> > > base;

    explicit test(const test_allocator<int>& a) : base(a) {}
    test(const container_type& container, const test_allocator<int>& a) : base(container, a) {}
#if TEST_STD_VER >= 11
    test(container_type&& container, const test_allocator<int>& a) : base(std::move(container), a) {}
    test(test&& q, const test_allocator<int>& a) : base(std::move(q), a) {}
#endif
    test_allocator<int> get_allocator() {return c.get_allocator();}
};

int tc_containers_container_adaptors_queue_queue_cons_alloc_ctor_alloc(void) {
    test q(test_allocator<int>(3));
    TC_ASSERT_EXPR(q.get_allocator() == test_allocator<int>(3));

  return 0;
}
