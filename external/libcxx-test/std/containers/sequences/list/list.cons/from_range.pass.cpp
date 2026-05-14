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
//   list(from_range_t, R&& rg, const Allocator& = Allocator()); // C++23

#include <list>

#include "../../from_range_sequence_containers.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_cons_from_range(void) {
  for_all_iterators_and_allocators<int>([]<class Iter, class Sent, class Alloc>() {
    test_sequence_container<std::list, int, Iter, Sent, Alloc>([](const auto&) {
      // No additional validation to do.
    });
  });
  test_sequence_container_move_only<std::list>();

  static_assert(test_constraints<std::list, int, double>());

  test_exception_safety_throwing_copy<std::list>();
  test_exception_safety_throwing_allocator<std::list, int>();

  return 0;
}

