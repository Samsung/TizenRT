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
// Some fields in the test case variables are deliberately not explicitly initialized, this silences a warning on GCC.
// ADDITIONAL_COMPILE_FLAGS: -Wno-missing-field-initializers

// <set>

// template<container-compatible-range<value_type> R>
//   void insert_range(R&& rg); // C++23

#include <unordered_set>

#include "../../insert_range_maps_sets.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_set_insert_range(void) {
  for_all_iterators_and_allocators<int, const int*>([]<class Iter, class Sent, class Alloc>() {
    test_map_set_insert_range<std::unordered_set<int, test_hash<int>, test_equal_to<int>, Alloc>, int, Iter, Sent>();
  });

  static_assert(test_set_constraints_insert_range<std::unordered_set, int, double>());

  test_set_insert_range_move_only<std::unordered_set>();

  test_set_insert_range_exception_safety_throwing_copy<std::unordered_set>();
  test_unord_set_insert_range_exception_safety_throwing_allocator<std::unordered_set, int>();

  return 0;
}
