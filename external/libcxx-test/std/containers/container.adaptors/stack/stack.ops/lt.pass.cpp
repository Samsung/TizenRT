//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <stack>

// template <class T, class Container>
//   bool operator< (const stack<T, Container>& x,const stack<T, Container>& y);
//
// template <class T, class Container>
//   bool operator> (const stack<T, Container>& x,const stack<T, Container>& y);
//
// template <class T, class Container>
//   bool operator>=(const stack<T, Container>& x,const stack<T, Container>& y);
//
// template <class T, class Container>
//   bool operator<=(const stack<T, Container>& x,const stack<T, Container>& y);

#include <stack>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class C>
C
make(int n)
{
    C c;
    for (int i = 0; i < n; ++i)
        c.push(i);
    return c;
}

int tc_containers_container_adaptors_stack_stack_ops_lt(void) {
    std::stack<int> q1 = make<std::stack<int> >(5);
    std::stack<int> q2 = make<std::stack<int> >(10);
    TC_ASSERT_EXPR(q1 < q2);
    TC_ASSERT_EXPR(q2 > q1);
    TC_ASSERT_EXPR(q1 <= q2);
    TC_ASSERT_EXPR(q2 >= q1);

  return 0;
}
