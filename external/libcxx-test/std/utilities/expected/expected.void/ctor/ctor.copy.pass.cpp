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

// constexpr expected(const expected& rhs);
//
// Effects: If rhs.has_value() is false, direct-non-list-initializes unex with rhs.error().
//
// Postconditions: rhs.has_value() == this->has_value().
//
// Throws: Any exception thrown by the initialization of unex.
//
// Remarks:
// - This constructor is defined as deleted unless is_copy_constructible_v<E> is true.
// - This constructor is trivial if is_trivially_copy_constructible_v<E> is true.

#include <cassert>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "../../types.h"
#include "libcxx_tc_common.h"

struct NonCopyable {
  NonCopyable(const NonCopyable&) = delete;
};

struct CopyableNonTrivial {
  int i;
  constexpr CopyableNonTrivial(int ii) : i(ii) {}
  constexpr CopyableNonTrivial(const CopyableNonTrivial& o) { i = o.i; }
  friend constexpr bool operator==(const CopyableNonTrivial&, const CopyableNonTrivial&) = default;
};

// Test: This constructor is defined as deleted unless is_copy_constructible_v<E> is true.
static_assert(std::is_copy_constructible_v<std::expected<void, int>>);
static_assert(std::is_copy_constructible_v<std::expected<void, CopyableNonTrivial>>);
static_assert(!std::is_copy_constructible_v<std::expected<void, NonCopyable>>);

// Test: This constructor is trivial if is_trivially_copy_constructible_v<E> is true.
static_assert(std::is_trivially_copy_constructible_v<std::expected<void, int>>);
static_assert(!std::is_trivially_copy_constructible_v<std::expected<void, CopyableNonTrivial>>);

constexpr bool test() {
  // copy the error non-trivial
  {
    const std::expected<void, CopyableNonTrivial> e1(std::unexpect, 5);
    auto e2 = e1;
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(e2.error().i == 5);
  }

  // copy the error trivial
  {
    const std::expected<void, int> e1(std::unexpect, 5);
    auto e2 = e1;
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(e2.error() == 5);
  }

  // copy TailClobberer as error
  {
    const std::expected<void, TailClobberer<1>> e1(std::unexpect);
    auto e2 = e1;
    TC_ASSERT_EXPR(!e2.has_value());
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  struct Throwing {
    Throwing() = default;
    Throwing(const Throwing&) { throw Except{}; }
  };

  // throw on copying error
  {
    const std::expected<void, Throwing> e1(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      [[maybe_unused]] auto e2 = e1;
      TC_ASSERT_EXPR(false);
    } catch (Except) {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_void_ctor_ctor_copy(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
