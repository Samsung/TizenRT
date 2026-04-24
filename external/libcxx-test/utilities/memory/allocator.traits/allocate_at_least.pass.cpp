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

// <memory>

// template<class Allocator>
// [[nodiscard]] constexpr allocation_result<typename allocator_traits<Allocator>::pointer>
//   allocate_at_least(Allocator& a, size_t n);

#include <cassert>
#include <concepts>
#include <cstddef>
#include <memory>
#include "libcxx_tc_common.h"

// check that std::allocation_result exists and isn't restricted to pointers
using AllocResult = std::allocation_result<int>;

template <class T>
struct no_allocate_at_least {
  using value_type = T;
  T t;

  constexpr T* allocate(std::size_t) { return &t; }
  constexpr void deallocate(T*, std::size_t) {}
};

template <class T>
struct has_allocate_at_least {
  using value_type = T;
  T t1;
  T t2;

  constexpr T* allocate(std::size_t) { return &t1; }
  constexpr void deallocate(T*, std::size_t) {}
  constexpr std::allocation_result<T*> allocate_at_least(std::size_t) {
    return {&t2, 2};
  }
};

constexpr bool test() {
  { // check that std::allocate_at_least forwards to allocator::allocate if no allocate_at_least exists
    no_allocate_at_least<int> alloc;
    std::same_as<std::allocation_result<int*>> decltype(auto) ret = std::allocate_at_least(alloc, 1);
    TC_ASSERT_EXPR(ret.count == 1);
    TC_ASSERT_EXPR(ret.ptr == &alloc.t);
  }

  { // check that std::allocate_at_least forwards to allocator::allocate_at_least if allocate_at_least exists
    has_allocate_at_least<int> alloc;
    std::same_as<std::allocation_result<int*>> decltype(auto) ret = std::allocate_at_least(alloc, 1);
    TC_ASSERT_EXPR(ret.count == 2);
    TC_ASSERT_EXPR(ret.ptr == &alloc.t2);
  }

  return true;
}

int tc_utilities_memory_allocator_traits_allocate_at_least(void) {
  test();
  static_assert(test());

  return 0;
}
