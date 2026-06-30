//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <stack>

// explicit stack(const Container&);

#include <stack>
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

int tc_containers_container_adaptors_stack_stack_cons_ctor_container(void) {
    typedef std::deque<int> Container;
    typedef std::stack<int> Q;
    Container d = make<Container>(5);
    Q q(d);
    TC_ASSERT_EXPR(q.size() == 5);
    for (std::size_t i = 0; i < d.size(); ++i)
    {
        TC_ASSERT_EXPR(q.top() == d[d.size() - i - 1]);
        q.pop();
    }

#if TEST_STD_VER >= 11
    static_assert(!test_convertible<Q, const Container&>(), "");
#endif

    return 0;
}
