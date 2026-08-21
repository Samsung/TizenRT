//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// template <class Alloc>
//     priority_queue(const priority_queue& q, const Alloc& a);

#include <queue>
#include <cassert>

template <class C>
C
make(int n)
{
    C c;
    for (int i = 0; i < n; ++i)
        c.push_back(i);
    return c;
}

#include "test_macros.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

template <class T>
struct test
    : public std::priority_queue<T, std::vector<T, test_allocator<T> > >
{
    typedef std::priority_queue<T, std::vector<T, test_allocator<T> > > base;
    typedef typename base::container_type container_type;
    typedef typename base::value_compare value_compare;

    explicit test(const test_allocator<int>& a) : base(a) {}
    test(const value_compare& compare, const test_allocator<int>& a)
        : base(compare, c, a) {}
    test(const value_compare& compare, const container_type& container,
         const test_allocator<int>& a) : base(compare, container, a) {}
    test(const test& q, const test_allocator<int>& a) : base(q, a) {}
    test_allocator<int> get_allocator() {return c.get_allocator();}

    using base::c;
};

int tc_containers_container_adaptors_priority_queue_priqueue_cons_alloc_ctor_copy_alloc(void) {
    test<int> qo(std::less<int>(),
                      make<std::vector<int, test_allocator<int> > >(5),
                      test_allocator<int>(2));
    test<int> q(qo, test_allocator<int>(6));
    TC_ASSERT_EXPR(q.size() == 5);
    TC_ASSERT_EXPR(q.c.get_allocator() == test_allocator<int>(6));
    TC_ASSERT_EXPR(q.top() == int(4));

  return 0;
}
