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
// <set>

// class multiset

// template <class InputIterator>
//     multiset(InputIterator first, InputIterator last,
//         const value_compare& comp, const allocator_type& a);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "../../../test_compare.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_iter_iter_alloc(void) {
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
    typedef test_less<V> C;
    typedef test_allocator<V> A;
    std::multiset<V, C, A> m(cpp17_input_iterator<const V*>(ar),
                        cpp17_input_iterator<const V*>(ar+sizeof(ar)/sizeof(ar[0])),
                        C(5), A(7));
    TC_ASSERT_EXPR(m.value_comp() == C(5));
    TC_ASSERT_EXPR(m.get_allocator() == A(7));
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
    }
#if TEST_STD_VER > 11
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
    typedef test_allocator<V> A;
    typedef test_less<int> C;
    A a;
    std::multiset<V, C, A> m(ar, ar+sizeof(ar)/sizeof(ar[0]), a);

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
    TC_ASSERT_EXPR(m.get_allocator() == a);
    }
#endif

  return 0;
}
