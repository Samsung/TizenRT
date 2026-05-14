//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class multiset

// iterator insert(const value_type& v);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template<class Container>
void do_insert_cv_test()
{
    typedef Container M;
    typedef typename M::iterator R;
    typedef typename M::value_type VT;
    M m;
    const VT v1(2);
    R r = m.insert(v1);
    TC_ASSERT_EXPR(r == m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(*r == 2);

    const VT v2(1);
    r = m.insert(v2);
    TC_ASSERT_EXPR(r == m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    TC_ASSERT_EXPR(*r == 1);

    const VT v3(3);
    r = m.insert(v3);
    TC_ASSERT_EXPR(r == std::prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(*r == 3);

    r = m.insert(v3);
    TC_ASSERT_EXPR(r == std::prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 4);
    TC_ASSERT_EXPR(*r == 3);
}

int tc_containers_associative_multiset_insert_cv(void) {
    do_insert_cv_test<std::multiset<int> >();
#if TEST_STD_VER >= 11
    {
        typedef std::multiset<int, std::less<int>, min_allocator<int>> M;
        do_insert_cv_test<M>();
    }
#endif

  return 0;
}
