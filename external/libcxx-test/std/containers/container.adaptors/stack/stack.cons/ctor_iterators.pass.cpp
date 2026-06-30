//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <queue>

// template <class InputIterator>
// stack(InputIterator, InputIterator);

#include <cassert>
#include <stack>

#include "test_allocator.h"
#include "libcxx_tc_common.h"

static_assert(!std::is_constructible_v<std::stack<int>, int, int, std::allocator<int>>);
static_assert(!std::is_constructible_v<std::stack<int>, int*, int*, int>);
static_assert( std::is_constructible_v<std::stack<int, std::deque<int, test_allocator<int>>>, int*, int*, test_allocator<int>>);
static_assert(!std::is_constructible_v<std::stack<int, std::deque<int, test_allocator<int>>>, int*, int*, std::allocator<int>>);

template <class T>
struct alloc : test_allocator<T> {
  template <class U>
  struct rebind {
    using other = alloc<U>;
  };
  alloc(test_allocator_statistics* a);
};
static_assert(
    std::is_constructible_v<std::stack<int, std::deque<int, alloc<int>>>, int*, int*, test_allocator_statistics*>);

int tc_containers_container_adaptors_stack_stack_cons_ctor_iterators(void) {
  const int a[] = {4, 3, 2, 1};
  std::stack<int> stack(a, a + 4);
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
