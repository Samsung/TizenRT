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

// map& operator=(initializer_list<value_type> il);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

void test_basic() {
  {
    typedef std::pair<const int, double> V;
    std::map<int, double> m =
                            {
                                {20, 1},
                            };
    m =
                            {
                                {1, 1},
                                {1, 1.5},
                                {1, 2},
                                {2, 1},
                                {2, 1.5},
                                {2, 2},
                                {3, 1},
                                {3, 1.5},
                                {3, 2}
                            };
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*std::next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*std::next(m.begin(), 2) == V(3, 1));
    }
    {
    typedef std::pair<const int, double> V;
    std::map<int, double, std::less<int>, min_allocator<V>> m =
                            {
                                {20, 1},
                            };
    m =
                            {
                                {1, 1},
                                {1, 1.5},
                                {1, 2},
                                {2, 1},
                                {2, 1.5},
                                {2, 2},
                                {3, 1},
                                {3, 1.5},
                                {3, 2}
                            };
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*std::next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*std::next(m.begin(), 2) == V(3, 1));
    }
}


void duplicate_keys_test() {
  test_allocator_statistics alloc_stats;
  typedef std::map<int, int, std::less<int>, test_allocator<std::pair<const int, int> > > Map;
  {
    LIBCPP_ASSERT(alloc_stats.alloc_count == 0);
    Map s({{1, 0}, {2, 0}, {3, 0}}, std::less<int>(), test_allocator<std::pair<const int, int> >(&alloc_stats));
    LIBCPP_ASSERT(alloc_stats.alloc_count == 3);
    s = {{4, 0}, {4, 0}, {4, 0}, {4, 0}};
    LIBCPP_ASSERT(alloc_stats.alloc_count == 1);
    TC_ASSERT_EXPR(s.size() == 1);
    TC_ASSERT_EXPR(s.begin()->first == 4);
  }
  LIBCPP_ASSERT(alloc_stats.alloc_count == 0);
}

int tc_containers_associative_map_map_cons_assign_initializer_list(void) {
  test_basic();
  duplicate_keys_test();

  return 0;
}
