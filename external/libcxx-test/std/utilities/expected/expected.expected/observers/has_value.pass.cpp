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

// constexpr bool has_value() const noexcept;

#include <cassert>
#include <concepts>
#include <expected>
#include <optional>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "../../types.h"
#include "libcxx_tc_common.h"

// Test noexcept
template <class T>
concept HasValueNoexcept =
    requires(T t) {
      { t.has_value() } noexcept;
    };

struct Foo {};
static_assert(!HasValueNoexcept<Foo>);

static_assert(HasValueNoexcept<std::expected<int, int>>);
static_assert(HasValueNoexcept<const std::expected<int, int>>);

constexpr bool test() {
  // has_value
  {
    const std::expected<int, int> e(5);
    TC_ASSERT_EXPR(e.has_value());
  }

  // !has_value
  {
    const std::expected<int, int> e(std::unexpect, 5);
    TC_ASSERT_EXPR(!e.has_value());
  }

  // The following tests check that the "has_value" flag is not overwritten
  // by the constructor of the value. This could happen because the flag is
  // stored in the tail padding of the value.
  //
  // The first test is a simplified version of the real code where this was
  // first observed.
  //
  // The other tests use a synthetic struct that clobbers its tail padding
  // on construction, making the issue easier to reproduce.
  //
  // See https://github.com/llvm/llvm-project/issues/68552 and the linked PR.
  {
    auto f1 = [] -> std::expected<std::optional<int>, long> { return 0; };

    auto f2 = [&f1] -> std::expected<std::optional<int>, int> {
      return f1().transform_error([](auto) { return 0; });
    };

    auto e = f2();
    TC_ASSERT_EXPR(e.has_value());
  }
  {
    const std::expected<TailClobberer<0>, bool> e = {};
    // clang-cl does not support [[no_unique_address]] yet.
#if !(defined(TEST_COMPILER_CLANG) && defined(_MSC_VER))
    LIBCPP_STATIC_ASSERT(sizeof(TailClobberer<0>) == sizeof(e));
#endif
    TC_ASSERT_EXPR(e.has_value());
  }

  return true;
}

int tc_utilities_expected_expected_expected_observers_has_value(void) {
  test();
  static_assert(test());
  return 0;
}
