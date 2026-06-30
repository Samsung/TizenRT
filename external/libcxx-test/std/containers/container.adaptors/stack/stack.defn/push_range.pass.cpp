//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20
// UNSUPPORTED: GCC-ALWAYS_INLINE-FIXME

// template<container-compatible-range<T> R>
//   void push_range(R&& rg); // C++23

#include <stack>

#include "../../push_range_container_adaptors.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_container_adaptors_stack_stack_defn_push_range(void) {
  for_all_iterators_and_allocators<int, const int*>([]<class Iter, class Sent, class Alloc>() {
    test_push_range<std::stack<int, std::deque<int, Alloc>>, Iter, Sent>();
  });
  test_push_range_move_only<std::stack>();
  test_push_range_inserter_choice<std::stack, int>();

  static_assert(test_constraints_push_range<std::stack, int, double>());

  test_push_range_exception_safety_throwing_copy<std::stack>();
  test_push_range_exception_safety_throwing_allocator<std::stack, std::deque, int>();

  return 0;
}
