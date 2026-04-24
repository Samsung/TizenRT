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

// C++2a[container.requirements.general]p8
//   Move constructors obtain an allocator by move construction from the allocator
//   belonging to the container being moved. Such move construction of the
//   allocator shall not exit via an exception.

#include <vector>
#include <deque>
#include <list>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>

#include "test_macros.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
void test(int expected_num_allocs = 1) {
  test_allocator_statistics alloc_stats;
  {
    alloc_stats.clear();
    using AllocT = typename C::allocator_type;
    C v(AllocT(42, 101, &alloc_stats));

    TC_ASSERT_EXPR(alloc_stats.count == expected_num_allocs);

    const int num_stored_allocs = alloc_stats.count;
    {
      const AllocT& a = v.get_allocator();
      TC_ASSERT_EXPR(alloc_stats.count == 1 + num_stored_allocs);
      TC_ASSERT_EXPR(a.get_data() == 42);
      TC_ASSERT_EXPR(a.get_id() == 101);
    }
    TC_ASSERT_EXPR(alloc_stats.count == num_stored_allocs);
    alloc_stats.clear_ctor_counters();

    C v2 = std::move(v);
    TC_ASSERT_EXPR(alloc_stats.count == num_stored_allocs * 2);
    TC_ASSERT_EXPR(alloc_stats.copied == 0);
    TC_ASSERT_EXPR(alloc_stats.moved == num_stored_allocs);
    {
      const AllocT& a = v.get_allocator();
      TC_ASSERT_EXPR(a.get_id() == test_alloc_base::moved_value);
      TC_ASSERT_EXPR(a.get_data() == test_alloc_base::moved_value);
    }
    {
      const AllocT& a = v2.get_allocator();
      TC_ASSERT_EXPR(a.get_id() == 101);
      TC_ASSERT_EXPR(a.get_data() == 42);
    }
  }
}

int tc_containers_container_requirements_container_requirements_general_allocator_move(void) {
  { // test sequence containers
    test<std::vector<int, test_allocator<int> > >();
    test<std::vector<bool, test_allocator<bool> > >();
    test<std::list<int, test_allocator<int> > >();
    test<std::forward_list<int, test_allocator<int> > >();

    // libc++ stores two allocators in deque
#ifdef _LIBCPP_VERSION
    int stored_allocators = 2;
#else
    int stored_allocators = 1;
#endif
    test<std::deque<int, test_allocator<int> > >(stored_allocators);
  }
  { // test associative containers
    test<std::set<int, std::less<int>, test_allocator<int> > >();
    test<std::multiset<int, std::less<int>, test_allocator<int> > >();

    using KV = std::pair<const int, int>;
    test<std::map<int, int, std::less<int>, test_allocator<KV> > >();
    test<std::multimap<int, int, std::less<int>, test_allocator<KV> > >();
  }
  { // test unordered containers
    // libc++ stores two allocators in the unordered containers.
#ifdef _LIBCPP_VERSION
    int stored_allocators = 2;
#else
    int stored_allocators = 1;
#endif
    test<std::unordered_set<int, std::hash<int>, std::equal_to<int>,
                            test_allocator<int> > >(stored_allocators);
    test<std::unordered_multiset<int, std::hash<int>, std::equal_to<int>,
                                 test_allocator<int> > >(stored_allocators);

    using KV = std::pair<const int, int>;
    test<std::unordered_map<int, int, std::hash<int>, std::equal_to<int>,
                            test_allocator<KV> > >(stored_allocators);
    test<std::unordered_multimap<int, int, std::hash<int>, std::equal_to<int>,
                                 test_allocator<KV> > >(stored_allocators);
  }

  return 0;
}
