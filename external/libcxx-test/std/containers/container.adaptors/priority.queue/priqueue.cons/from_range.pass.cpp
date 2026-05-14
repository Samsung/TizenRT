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
#include "../../../test_compare.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// template <container-compatible-range<T> R>
//   priority_queue(from_range_t, R&& rg, const Compare& x = Compare()); // since C++23
// template <container-compatible-range<T> R, class Alloc>
//   priority_queue(from_range_t, R&& rg, const Compare&, const Alloc&); // since C++23
// template <container-compatible-range<T> R, class Alloc>
//   priority_queue(from_range_t, R&& rg, const Alloc&); // since C++23

template <class Range>
concept PriorityQueueHasFromRangeCtr = requires (Range&& range) {
  std::priority_queue<int>(std::from_range, std::forward<Range>(range));
  std::priority_queue<int>(std::from_range, std::forward<Range>(range), std::less<int>());
  std::priority_queue<int>(std::from_range, std::forward<Range>(range), std::less<int>(), std::allocator<int>());
  std::priority_queue<int>(std::from_range, std::forward<Range>(range), std::allocator<int>());
};

constexpr bool test_constraints_priority_queue() {
  // Input range with the same value type.
  static_assert(PriorityQueueHasFromRangeCtr<InputRange<int>>);
  // Input range with a convertible value type.
  static_assert(PriorityQueueHasFromRangeCtr<InputRange<double>>);
  // Input range with a non-convertible value type.
  static_assert(!PriorityQueueHasFromRangeCtr<InputRange<Empty>>);
  // Not an input range.
  static_assert(!PriorityQueueHasFromRangeCtr<InputRangeNotDerivedFrom>);
  static_assert(!PriorityQueueHasFromRangeCtr<InputRangeNotIndirectlyReadable>);
  static_assert(!PriorityQueueHasFromRangeCtr<InputRangeNotInputOrOutputIterator>);

  return true;
}

int tc_containers_container_adaptors_priority_queue_priqueue_cons_from_range(void) {
  for_all_iterators_and_allocators<int>([]<class Iter, class Sent, class Alloc>() {
    test_priority_queue<std::vector, int, Iter, Sent, test_less<int>, Alloc>();
  });
  test_container_adaptor_move_only<std::priority_queue>();

  static_assert(test_constraints_priority_queue());

  test_exception_safety_throwing_copy<std::priority_queue>();
  test_exception_safety_throwing_allocator<std::priority_queue, int>();

  return 0;
}
