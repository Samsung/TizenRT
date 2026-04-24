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

#include <vector>

#include "../from_range_sequence_containers.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// template<container-compatible-range<T> R>
//   vector(from_range_t, R&& rg, const Allocator& = Allocator()); // C++23

constexpr bool test() {
  for_all_iterators_and_allocators<bool>([]<class Iter, class Sent, class Alloc>() {
    test_vector_bool<Iter, Sent, Alloc>([](const auto& c) {
      LIBCPP_ASSERT(c.__invariants());
      // `is_contiguous_container_asan_correct` doesn't work on `vector<bool>`.
    });
  });

  return true;
}

int tc_containers_sequences_vector_bool_construct_from_range(void) {
  test();
  static_assert(test());

  static_assert(test_constraints<std::vector, bool, char>());

  // Note: test_exception_safety_throwing_copy doesn't apply because copying a boolean cannot throw.
  test_exception_safety_throwing_allocator<std::vector, bool>();

  return 0;
}
