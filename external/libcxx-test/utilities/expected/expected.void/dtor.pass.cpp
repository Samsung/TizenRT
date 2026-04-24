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
// Older Clangs do not support the C++20 feature to constrain destructors
// XFAIL: apple-clang-14

// constexpr ~expected();
//
// Effects: If has_value() is false, destroys unex.
//
// Remarks: If is_trivially_destructible_v<E> is true, then this destructor is a trivial destructor.

#include <cassert>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test Remarks: If is_trivially_destructible_v<E> is true, then this destructor is a trivial destructor.
struct NonTrivial {
  ~NonTrivial() {}
};

static_assert(std::is_trivially_destructible_v<std::expected<void, int>>);
static_assert(!std::is_trivially_destructible_v<std::expected<void, NonTrivial>>);

struct TrackedDestroy {
  bool& destroyed;
  constexpr TrackedDestroy(bool& b) : destroyed(b) {}
  constexpr ~TrackedDestroy() { destroyed = true; }
};

constexpr bool test() {
  // has value
  { [[maybe_unused]] std::expected<void, TrackedDestroy> e(std::in_place); }

  // has error
  {
    bool errorDestroyed = false;
    { [[maybe_unused]] std::expected<void, TrackedDestroy> e(std::unexpect, errorDestroyed); }
    TC_ASSERT_EXPR(errorDestroyed);
  }

  return true;
}

int tc_utilities_expected_expected_void_dtor(void) {
  test();
  static_assert(test());
  return 0;
}
