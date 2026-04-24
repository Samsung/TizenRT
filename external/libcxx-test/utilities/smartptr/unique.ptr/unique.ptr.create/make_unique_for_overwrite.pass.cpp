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

// template<class T>
//   constexpr unique_ptr<T> make_unique_for_overwrite(); // T is not array
//
// template<class T>
//   constexpr unique_ptr<T> make_unique_for_overwrite(size_t n); // T is U[]
//
// template<class T, class... Args>
//   unspecified make_unique_for_overwrite(Args&&...) = delete; // T is U[N]

#include <cassert>
#include <concepts>
#include <cstring>
#include <memory>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class... Args>
concept HasMakeUniqueForOverwrite =
    requires(Args&&... args) { std::make_unique_for_overwrite<T>(std::forward<Args>(args)...); };

struct Foo {
  int i;
};

// template<class T>
//   constexpr unique_ptr<T> make_unique_for_overwrite();
static_assert(HasMakeUniqueForOverwrite<int>);
static_assert(HasMakeUniqueForOverwrite<Foo>);
static_assert(!HasMakeUniqueForOverwrite<int, int>);
static_assert(!HasMakeUniqueForOverwrite<Foo, Foo>);

// template<class T>
//   constexpr unique_ptr<T> make_unique_for_overwrite(size_t n);
static_assert(HasMakeUniqueForOverwrite<int[], std::size_t>);
static_assert(HasMakeUniqueForOverwrite<Foo[], std::size_t>);
static_assert(!HasMakeUniqueForOverwrite<int[]>);
static_assert(!HasMakeUniqueForOverwrite<Foo[]>);
static_assert(!HasMakeUniqueForOverwrite<int[], std::size_t, int>);
static_assert(!HasMakeUniqueForOverwrite<Foo[], std::size_t, int>);

// template<class T, class... Args>
//   unspecified make_unique_for_overwrite(Args&&...) = delete;
static_assert(!HasMakeUniqueForOverwrite<int[2]>);
static_assert(!HasMakeUniqueForOverwrite<int[2], std::size_t>);
static_assert(!HasMakeUniqueForOverwrite<int[2], int>);
static_assert(!HasMakeUniqueForOverwrite<int[2], int, int>);
static_assert(!HasMakeUniqueForOverwrite<Foo[2]>);
static_assert(!HasMakeUniqueForOverwrite<Foo[2], std::size_t>);
static_assert(!HasMakeUniqueForOverwrite<Foo[2], int>);
static_assert(!HasMakeUniqueForOverwrite<Foo[2], int, int>);

struct WithDefaultConstructor {
  int i;
  constexpr WithDefaultConstructor() : i(5) {}
};

TEST_CONSTEXPR_CXX23 bool test() {
  // single int
  {
    std::same_as<std::unique_ptr<int>> decltype(auto) ptr = std::make_unique_for_overwrite<int>();
    // memory is available for write, otherwise constexpr test would fail
    *ptr = 5;
  }

  // unbounded array int[]
  {
    std::same_as<std::unique_ptr<int[]>> decltype(auto) ptrs = std::make_unique_for_overwrite<int[]>(3);

    // memory is available for write, otherwise constexpr test would fail
    ptrs[0] = 3;
    ptrs[1] = 4;
    ptrs[2] = 5;
  }

  // single with default constructor
  {
    std::same_as<std::unique_ptr<WithDefaultConstructor>> decltype(auto) ptr =
        std::make_unique_for_overwrite<WithDefaultConstructor>();
    TC_ASSERT_EXPR(ptr->i == 5);
  }

  // unbounded array with default constructor
  {
    std::same_as<std::unique_ptr<WithDefaultConstructor[]>> decltype(auto) ptrs =
        std::make_unique_for_overwrite<WithDefaultConstructor[]>(3);
    TC_ASSERT_EXPR(ptrs[0].i == 5);
    TC_ASSERT_EXPR(ptrs[1].i == 5);
    TC_ASSERT_EXPR(ptrs[2].i == 5);
  }

  return true;
}

// The standard specifically says to use `new (p) T`, which means that we should pick up any
// custom in-class operator new if there is one.
struct WithCustomNew {
  inline static bool customNewCalled    = false;
  inline static bool customNewArrCalled = false;

  static void* operator new(std::size_t n) {
    customNewCalled = true;
    return ::operator new(n);
    ;
  }

  static void* operator new[](std::size_t n) {
    customNewArrCalled = true;
    return ::operator new[](n);
  }
};

void testCustomNew() {
  // single with custom operator new
  {
    [[maybe_unused]] std::same_as<std::unique_ptr<WithCustomNew>> decltype(auto) ptr =
        std::make_unique_for_overwrite<WithCustomNew>();

    TC_ASSERT_EXPR(WithCustomNew::customNewCalled);
  }

  // unbounded array with custom operator new
  {
    [[maybe_unused]] std::same_as<std::unique_ptr<WithCustomNew[]>> decltype(auto) ptr =
        std::make_unique_for_overwrite<WithCustomNew[]>(3);

    TC_ASSERT_EXPR(WithCustomNew::customNewArrCalled);
  }
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_for_overwrite(void) {
  test();
  testCustomNew();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
