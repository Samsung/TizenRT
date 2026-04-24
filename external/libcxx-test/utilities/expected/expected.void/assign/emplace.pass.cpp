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

// constexpr void emplace() noexcept;
//
// Effects: If has_value() is false, destroys unex and sets has_val to true.

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "../../types.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
concept EmplaceNoexcept =
    requires(T t) {
      { t.emplace() } noexcept;
    };
static_assert(!EmplaceNoexcept<int>);

static_assert(EmplaceNoexcept<std::expected<void, int>>);

constexpr bool test() {
  // has_value
  {
    std::expected<void, int> e;
    e.emplace();
    TC_ASSERT_EXPR(e.has_value());
  }

  // !has_value
  {
    Traced::state state{};
    std::expected<int, Traced> e(std::unexpect, state, 5);
    e.emplace();

    TC_ASSERT_EXPR(state.dtorCalled);
    TC_ASSERT_EXPR(e.has_value());
  }

  return true;
}

int tc_utilities_expected_expected_void_assign_emplace(void) {
  test();
  static_assert(test());
  return 0;
}
