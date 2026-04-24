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

#include <queue>

#include "../../from_range_container_adaptors.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// template<container-compatible-range<T> R> queue(from_range_t, R&& rg); // since C++23
// template<container-compatible-range<T> R, class Alloc>
//   queue(from_range_t, R&& rg, const Alloc&); // since C++23

int tc_containers_container_adaptors_queue_queue_cons_from_range(void) {
  for_all_iterators_and_allocators<int>([]<class Iter, class Sent, class Alloc>() {
    test_container_adaptor<std::queue, std::deque, int, Iter, Sent, Alloc>();
  });
  test_container_adaptor_move_only<std::queue>();

  static_assert(test_constraints<std::queue, int, double>());

  test_exception_safety_throwing_copy<std::queue>();
  test_exception_safety_throwing_allocator<std::queue, int>();

  return 0;
}
