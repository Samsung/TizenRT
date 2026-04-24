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
// UNSUPPORTED: GCC-ALWAYS_INLINE-FIXME

// template<container-compatible-range<T> R>
//   constexpr iterator insert_range(const_iterator position, R&& rg); // C++23

#include <deque>

#include "../../insert_range_sequence_containers.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// Tested cases:
// - different kinds of insertions (inserting an {empty/one-element/mid-sized/long range} into an
//   {empty/one-element/full} container at the {beginning/middle/end});
// - inserting move-only elements;
// - an exception is thrown when copying the elements or when allocating new elements.
int tc_containers_sequences_deque_deque_modifiers_insert_range(void) {
  static_assert(test_constraints_insert_range<std::deque, int, double>());

  for_all_iterators_and_allocators<int, const int*>([]<class Iter, class Sent, class Alloc>() {
    test_sequence_insert_range<std::deque<int, Alloc>, Iter, Sent>([](auto&& c) {
      LIBCPP_ASSERT(c.__invariants());
    });
  });
  test_sequence_insert_range_move_only<std::deque>();

  test_insert_range_exception_safety_throwing_copy<std::deque>();
  test_insert_range_exception_safety_throwing_allocator<std::deque, int>();

  return 0;
}
