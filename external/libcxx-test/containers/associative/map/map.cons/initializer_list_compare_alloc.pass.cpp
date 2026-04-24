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
// UNSUPPORTED: c++03

// <map>

// class map

// map(initializer_list<value_type> il, const key_compare& comp, const allocator_type& a);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_cons_initializer_list_compare_alloc(void) {
    {
    typedef std::pair<const int, double> V;
    typedef test_less<int> C;
    typedef test_allocator<std::pair<const int, double> > A;
    std::map<int, double, C, A> m({
                                   {1, 1},
                                   {1, 1.5},
                                   {1, 2},
                                   {2, 1},
                                   {2, 1.5},
                                   {2, 2},
                                   {3, 1},
                                   {3, 1.5},
                                   {3, 2}
                                  }, C(3), A(6));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*std::next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*std::next(m.begin(), 2) == V(3, 1));
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    TC_ASSERT_EXPR(m.get_allocator() == A(6));
    }
    {
    typedef std::pair<const int, double> V;
    typedef test_less<int> C;
    typedef min_allocator<std::pair<const int, double> > A;
    std::map<int, double, C, A> m({
                                   {1, 1},
                                   {1, 1.5},
                                   {1, 2},
                                   {2, 1},
                                   {2, 1.5},
                                   {2, 2},
                                   {3, 1},
                                   {3, 1.5},
                                   {3, 2}
                                  }, C(3), A());
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*std::next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*std::next(m.begin(), 2) == V(3, 1));
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    TC_ASSERT_EXPR(m.get_allocator() == A());
    }
    {
    typedef std::pair<const int, double> V;
    typedef min_allocator<V> A;
    typedef test_less<int> C;
    typedef std::map<int, double, C, A> M;
    A a;
    M m ({ {1, 1},
           {1, 1.5},
           {1, 2},
           {2, 1},
           {2, 1.5},
           {2, 2},
           {3, 1},
           {3, 1.5},
           {3, 2}
          }, a);

    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*std::next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*std::next(m.begin(), 2) == V(3, 1));
    TC_ASSERT_EXPR(m.get_allocator() == a);
    }
    {
    typedef std::pair<const int, double> V;
    typedef explicit_allocator<V> A;
    typedef test_less<int> C;
    A a;
    std::map<int, double, C, A> m({
                                   {1, 1},
                                   {1, 1.5},
                                   {1, 2},
                                   {2, 1},
                                   {2, 1.5},
                                   {2, 2},
                                   {3, 1},
                                   {3, 1.5},
                                   {3, 2}
                                  }, C(3), a);
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*std::next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*std::next(m.begin(), 2) == V(3, 1));
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    TC_ASSERT_EXPR(m.get_allocator() == a);
    }

  return 0;
}
