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

// <queue>

// template <class InputIterator, class Allocator>
// stack(InputIterator, InputIterator, Allocator);

#include <cassert>
#include <stack>

#include "test_allocator.h"
#include "libcxx_tc_common.h"

using base_type = std::stack<int, std::deque<int, test_allocator<int>>>;

class GetAlloc : public base_type {
  test_allocator_statistics* stats;

public:
  GetAlloc(test_allocator_statistics& stats_, const int* begin, const int* end)
      : base_type(begin, end, test_allocator<int>(&stats_)), stats(&stats_) {}
  void check() {
    TC_ASSERT_EXPR(size() == 4);
    TC_ASSERT_EXPR(stats->alloc_count > 0);
  }
};

int tc_containers_container_adaptors_stack_stack_cons_alloc_ctor_iterators(void) {
  const int a[] = {4, 3, 2, 1};
  test_allocator_statistics stats{};
  GetAlloc stack(stats, a, a + 4);
  TC_ASSERT_EXPR(stack.top() == 1);
  stack.pop();
  TC_ASSERT_EXPR(stack.top() == 2);
  stack.pop();
  TC_ASSERT_EXPR(stack.top() == 3);
  stack.pop();
  TC_ASSERT_EXPR(stack.top() == 4);
  stack.pop();
  TC_ASSERT_EXPR(stack.empty());

  return 0;
}
