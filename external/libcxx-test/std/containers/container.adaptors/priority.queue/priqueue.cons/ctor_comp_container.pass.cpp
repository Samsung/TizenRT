//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <queue>

// priority_queue(const Compare& comp, const Container& c);

#include <queue>
#include <cassert>
#include <functional>

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

int tc_containers_container_adaptors_priority_queue_priqueue_cons_ctor_comp_container(void) {
    typedef std::vector<int> Container;
    typedef std::greater<int> Compare;
    typedef std::priority_queue<int, Container, Compare> Q;
    Container v = make<Container>(5);
    Q q(Compare(), v);
    TC_ASSERT_EXPR(q.size() == 5);
    TC_ASSERT_EXPR(q.top() == 0);

#if TEST_STD_VER >= 11
    // It should be explicit, so not convertible before C++20.
    static_assert(test_convertible<Q, const Compare&, const Container&>(), "");
#endif

    return 0;
}
