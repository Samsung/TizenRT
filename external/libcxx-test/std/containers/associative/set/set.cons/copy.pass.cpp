//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class set

// set(const set& m);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_set_cons_copy(void) {
    {
        typedef int V;
        V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3
        };
        typedef test_less<int> C;
        typedef test_allocator<V> A;
        std::set<int, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(7));
        std::set<int, C, A> m = mo;
        TC_ASSERT_EXPR(m.get_allocator() == A(7));
        TC_ASSERT_EXPR(m.key_comp() == C(5));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
        TC_ASSERT_EXPR(*m.begin() == 1);
        TC_ASSERT_EXPR(*std::next(m.begin()) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);

        TC_ASSERT_EXPR(mo.get_allocator() == A(7));
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
        TC_ASSERT_EXPR(mo.size() == 3);
        TC_ASSERT_EXPR(std::distance(mo.begin(), mo.end()) == 3);
        TC_ASSERT_EXPR(*mo.begin() == 1);
        TC_ASSERT_EXPR(*std::next(mo.begin()) == 2);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 2) == 3);
    }
#if TEST_STD_VER >= 11
    {
        typedef int V;
        V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3
        };
        typedef test_less<int> C;
        typedef other_allocator<V> A;
        std::set<int, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(7));
        std::set<int, C, A> m = mo;
        TC_ASSERT_EXPR(m.get_allocator() == A(-2));
        TC_ASSERT_EXPR(m.key_comp() == C(5));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
        TC_ASSERT_EXPR(*m.begin() == 1);
        TC_ASSERT_EXPR(*std::next(m.begin()) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);

        TC_ASSERT_EXPR(mo.get_allocator() == A(7));
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
        TC_ASSERT_EXPR(mo.size() == 3);
        TC_ASSERT_EXPR(std::distance(mo.begin(), mo.end()) == 3);
        TC_ASSERT_EXPR(*mo.begin() == 1);
        TC_ASSERT_EXPR(*std::next(mo.begin()) == 2);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 2) == 3);
    }
#endif

  return 0;
}
