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
//                  const Compare& comp, Container&& c, const Alloc& a);

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

template<class T, class Cont, class Comp = std::less<T> >
struct PQ : std::priority_queue<T, Cont, Comp> {
    typedef std::priority_queue<T, Cont, Comp> base;

    template<class It, class Alloc>
    explicit PQ(It first, It last, const Comp& compare, Cont&& v, const Alloc& a) : base(first, last, compare, std::move(v), a) {}

    using base::c;
};

int tc_containers_container_adaptors_priority_queue_priqueue_cons_alloc_ctor_iter_iter_comp_rcont_alloc(void) {
    using Alloc = test_allocator<MoveOnly>;
    int a[] = {3, 5, 2, 0, 6, 8, 1};
    PQ<MoveOnly, std::vector<MoveOnly, Alloc>> q(
        a+3, a+7, std::less<MoveOnly>(),
        std::vector<MoveOnly, Alloc>(a, a+3), Alloc(2));
    TC_ASSERT_EXPR(q.size() == 7);
    TC_ASSERT_EXPR(q.top() == MoveOnly(8));
    TC_ASSERT_EXPR(q.c.get_allocator() == Alloc(2));

    return 0;
}
