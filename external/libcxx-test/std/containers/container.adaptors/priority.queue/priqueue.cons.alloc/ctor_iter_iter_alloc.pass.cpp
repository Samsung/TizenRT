//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// template <class InputIterator>
//   priority_queue(InputIterator first, InputIterator last, const Alloc& a);

#include <queue>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

template<class T, class Cont, class Comp = std::less<T> >
struct PQ : std::priority_queue<T, Cont, Comp> {
    typedef std::priority_queue<T, Cont, Comp> base;

    template<class It, class Alloc>
    explicit PQ(It first, It last, const Alloc& a) : base(first, last, a) {}

    using base::c;
};

int tc_containers_container_adaptors_priority_queue_priqueue_cons_alloc_ctor_iter_iter_alloc(void) {
    int a[] = {3, 5, 2, 0, 6, 8, 1};
    typedef test_allocator<int> Alloc;
    PQ<int, std::vector<int, Alloc> > q(a, a+7, Alloc(2));
    TC_ASSERT_EXPR(q.size() == 7);
    TC_ASSERT_EXPR(q.top() == 8);
    TC_ASSERT_EXPR(q.c.get_allocator() == Alloc(2));

    return 0;
}
