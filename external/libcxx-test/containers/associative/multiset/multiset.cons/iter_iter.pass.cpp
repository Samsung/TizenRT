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
//     multiset(InputIterator first, InputIterator last);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_iter_iter(void) {
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
    std::multiset<V> m(cpp17_input_iterator<const int*>(ar),
                  cpp17_input_iterator<const int*>(ar+sizeof(ar)/sizeof(ar[0])));
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
    std::multiset<V, std::less<V>, min_allocator<V>> m(cpp17_input_iterator<const int*>(ar),
                  cpp17_input_iterator<const int*>(ar+sizeof(ar)/sizeof(ar[0])));
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
#endif

  return 0;
}
