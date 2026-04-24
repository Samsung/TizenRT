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
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// template<container-compatible-range<T> R>
//   void push_range(R&& rg); // C++23

#include <queue>

#include "../../push_range_container_adaptors.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_priority_queue_priqueue_members_push_range(void) {
  for_all_iterators_and_allocators<int, const int*>([]<class Iter, class Sent, class Alloc>() {
    test_push_range<std::priority_queue<int, std::vector<int, Alloc>>, Iter, Sent>(/*is_result_heapified=*/true);
  });
  test_push_range_move_only<std::priority_queue>();
  test_push_range_inserter_choice<std::priority_queue, int>(/*is_result_heapified=*/true);

  static_assert(test_constraints_push_range<std::priority_queue, int, double>());

  test_push_range_exception_safety_throwing_copy<std::priority_queue>();
  test_push_range_exception_safety_throwing_allocator<std::priority_queue, std::vector, int>();

  return 0;
}

