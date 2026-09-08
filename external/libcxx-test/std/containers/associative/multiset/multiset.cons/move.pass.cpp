//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <set>

// class multiset

// multiset(multiset&& s);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_move(void) {
    {
        typedef int V;
        typedef test_less<int> C;
        typedef test_allocator<V> A;
        std::multiset<int, C, A> mo(C(5), A(7));
        std::multiset<int, C, A> m = std::move(mo);
        TC_ASSERT_EXPR(m.get_allocator() == A(7));
        TC_ASSERT_EXPR(m.key_comp() == C(5));
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 0);

        TC_ASSERT_EXPR(mo.get_allocator() == A(test_alloc_base::moved_value));
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
        TC_ASSERT_EXPR(mo.size() == 0);
        TC_ASSERT_EXPR(std::distance(mo.begin(), mo.end()) == 0);
    }
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
        std::multiset<int, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(7));
        std::multiset<int, C, A> m = std::move(mo);
        TC_ASSERT_EXPR(m.get_allocator() == A(7));
        TC_ASSERT_EXPR(m.key_comp() == C(5));
        TC_ASSERT_EXPR(m.size() == 9);
        TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 9);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 7) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 8) == 3);

        TC_ASSERT_EXPR(mo.get_allocator() == A(test_alloc_base::moved_value));
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
        TC_ASSERT_EXPR(mo.size() == 0);
        TC_ASSERT_EXPR(std::distance(mo.begin(), mo.end()) == 0);
    }
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
        typedef min_allocator<V> A;
        std::multiset<int, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A());
        std::multiset<int, C, A> m = std::move(mo);
        TC_ASSERT_EXPR(m.get_allocator() == A());
        TC_ASSERT_EXPR(m.key_comp() == C(5));
        TC_ASSERT_EXPR(m.size() == 9);
        TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 9);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 7) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 8) == 3);

        TC_ASSERT_EXPR(mo.get_allocator() == A());
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
        TC_ASSERT_EXPR(mo.size() == 0);
        TC_ASSERT_EXPR(std::distance(mo.begin(), mo.end()) == 0);
    }

  return 0;
}
