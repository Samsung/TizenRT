//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <queue>

// explicit priority_queue(const Compare& comp, Container&& c); // before C++20
// priority_queue(const Compare& comp, Container&& c);          // C++20

#include <queue>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "test_convertible.h"
#include "libcxx_tc_common.h"

template <class C>
C
make(int n)
{
    C c;
    for (int i = 0; i < n; ++i)
        c.push_back(MoveOnly(i));
    return c;
}

int tc_containers_container_adaptors_priority_queue_priqueue_cons_ctor_comp_rcontainer(void) {
    typedef std::vector<MoveOnly> Container;
    typedef std::less<MoveOnly> Compare;
    typedef std::priority_queue<MoveOnly> Q;
    Q q(Compare(), make<Container>(5));
    TC_ASSERT_EXPR(q.size() == 5);
    TC_ASSERT_EXPR(q.top() == MoveOnly(4));

    static_assert(test_convertible<Q, const Compare&, Container&&>(), "");

    return 0;
}
