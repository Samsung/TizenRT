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
// <map>

// class map

// template <class InputIterator>
//   void insert(InputIterator first, InputIterator last);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_modifiers_insert_iter_iter(void) {
    {
        typedef std::map<int, double> M;
        typedef std::pair<int, double> P;
        P ar[] =
        {
            P(1, 1),
            P(1, 1.5),
            P(1, 2),
            P(2, 1),
            P(2, 1.5),
            P(2, 2),
            P(3, 1),
            P(3, 1.5),
            P(3, 2),
        };
        M m;
        m.insert(cpp17_input_iterator<P*>(ar), cpp17_input_iterator<P*>(ar + sizeof(ar)/sizeof(ar[0])));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == 1);
        TC_ASSERT_EXPR(std::next(m.begin())->first == 2);
        TC_ASSERT_EXPR(std::next(m.begin())->second == 1);
        TC_ASSERT_EXPR(std::next(m.begin(), 2)->first == 3);
        TC_ASSERT_EXPR(std::next(m.begin(), 2)->second == 1);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::map<int, double, std::less<int>, min_allocator<std::pair<const int, double>>> M;
        typedef std::pair<int, double> P;
        P ar[] =
        {
            P(1, 1),
            P(1, 1.5),
            P(1, 2),
            P(2, 1),
            P(2, 1.5),
            P(2, 2),
            P(3, 1),
            P(3, 1.5),
            P(3, 2),
        };
        M m;
        m.insert(cpp17_input_iterator<P*>(ar), cpp17_input_iterator<P*>(ar + sizeof(ar)/sizeof(ar[0])));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == 1);
        TC_ASSERT_EXPR(std::next(m.begin())->first == 2);
        TC_ASSERT_EXPR(std::next(m.begin())->second == 1);
        TC_ASSERT_EXPR(std::next(m.begin(), 2)->first == 3);
        TC_ASSERT_EXPR(std::next(m.begin(), 2)->second == 1);
    }
#endif

  return 0;
}
