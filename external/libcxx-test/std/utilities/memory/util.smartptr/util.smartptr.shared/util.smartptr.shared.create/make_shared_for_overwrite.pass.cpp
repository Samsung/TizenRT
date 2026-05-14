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
// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: sanitizer-new-delete

// template<class T>
//   shared_ptr<T> make_shared_for_overwrite(); // T is not U[]
//
// template<class T>
//   shared_ptr<T> make_shared_for_overwrite(size_t N); // T is U[]

#include <cassert>
#include <concepts>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class... Args>
concept HasMakeSharedForOverwrite =
    requires(Args&&... args) { std::make_shared_for_overwrite<T>(std::forward<Args>(args)...); };

struct Foo {
  int i;
};

// non array
static_assert(HasMakeSharedForOverwrite<int>);
static_assert(HasMakeSharedForOverwrite<Foo>);
static_assert(!HasMakeSharedForOverwrite<int, int>);
static_assert(!HasMakeSharedForOverwrite<Foo, Foo>);

// bounded array
static_assert(HasMakeSharedForOverwrite<int[2]>);
static_assert(!HasMakeSharedForOverwrite<int[2], std::size_t>);
static_assert(!HasMakeSharedForOverwrite<int[2], int>);
static_assert(!HasMakeSharedForOverwrite<int[2], int, int>);
static_assert(HasMakeSharedForOverwrite<Foo[2]>);
static_assert(!HasMakeSharedForOverwrite<Foo[2], std::size_t>);
static_assert(!HasMakeSharedForOverwrite<Foo[2], int>);
static_assert(!HasMakeSharedForOverwrite<Foo[2], int, int>);

// unbounded array
static_assert(HasMakeSharedForOverwrite<int[], std::size_t>);
static_assert(HasMakeSharedForOverwrite<Foo[], std::size_t>);
static_assert(!HasMakeSharedForOverwrite<int[]>);
static_assert(!HasMakeSharedForOverwrite<Foo[]>);
static_assert(!HasMakeSharedForOverwrite<int[], std::size_t, int>);
static_assert(!HasMakeSharedForOverwrite<Foo[], std::size_t, int>);

constexpr char pattern = 0xDE;

void* operator new(std::size_t count) {
  void* ptr = std::malloc(count);
  for (std::size_t i = 0; i < count; ++i) {
    *(reinterpret_cast<char*>(ptr) + i) = pattern;
  }
  return ptr;
}

void* operator new[](std::size_t count) { return ::operator new(count); }

void operator delete(void* ptr) noexcept { std::free(ptr); }

void operator delete[](void* ptr) noexcept { ::operator delete(ptr); }

struct WithDefaultConstructor {
  int i;
  constexpr WithDefaultConstructor() : i(5) {}
};

bool test() {
  // single int
  {
    std::same_as<std::shared_ptr<int>> decltype(auto) ptr = std::make_shared_for_overwrite<int>();
    TC_ASSERT_EXPR(*(reinterpret_cast<char*>(ptr.get())) == pattern);
  }

  // bounded array int[N]
  {
    std::same_as<std::shared_ptr<int[2]>> decltype(auto) ptr = std::make_shared_for_overwrite<int[2]>();
    TC_ASSERT_EXPR(*(reinterpret_cast<char*>(&ptr[0])) == pattern);
    TC_ASSERT_EXPR(*(reinterpret_cast<char*>(&ptr[1])) == pattern);
  }

  // unbounded array int[]
  {
    std::same_as<std::shared_ptr<int[]>> decltype(auto) ptr = std::make_shared_for_overwrite<int[]>(3);
    TC_ASSERT_EXPR(*(reinterpret_cast<char*>(&ptr[0])) == pattern);
    TC_ASSERT_EXPR(*(reinterpret_cast<char*>(&ptr[1])) == pattern);
    TC_ASSERT_EXPR(*(reinterpret_cast<char*>(&ptr[2])) == pattern);
  }

  // single with default constructor
  {
    std::same_as<std::shared_ptr<WithDefaultConstructor>> decltype(auto) ptr =
        std::make_shared_for_overwrite<WithDefaultConstructor>();
    TC_ASSERT_EXPR(ptr->i == 5);
  }

  // bounded array with default constructor
  {
    std::same_as<std::shared_ptr<WithDefaultConstructor[2]>> decltype(auto) ptr =
        std::make_shared_for_overwrite<WithDefaultConstructor[2]>();
    TC_ASSERT_EXPR(ptr[0].i == 5);
    TC_ASSERT_EXPR(ptr[1].i == 5);
  }

  // unbounded array with default constructor
  {
    std::same_as<std::shared_ptr<WithDefaultConstructor[]>> decltype(auto) ptrs =
        std::make_shared_for_overwrite<WithDefaultConstructor[]>(3);
    TC_ASSERT_EXPR(ptrs[0].i == 5);
    TC_ASSERT_EXPR(ptrs[1].i == 5);
    TC_ASSERT_EXPR(ptrs[2].i == 5);
  }

  return true;
}

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_create_make_shared_for_overwrite(void) {
  test();

  return 0;
}
