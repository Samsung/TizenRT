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
// <optional>

// template<class F> constexpr optional or_else(F&&) &&;
// template<class F> constexpr optional or_else(F&&) const&;

#include "MoveOnly.h"

#include <cassert>
#include <optional>
#include "libcxx_tc_common.h"

struct NonMovable {
  NonMovable() = default;
  NonMovable(NonMovable&&) = delete;
};

template <class Opt, class F>
concept has_or_else = requires(Opt&& opt, F&& f) {
  {std::forward<Opt>(opt).or_else(std::forward<F>(f))};
};

template <class T>
std::optional<T> return_optional() {}

static_assert(has_or_else<std::optional<int>&, decltype(return_optional<int>)>);
static_assert(has_or_else<std::optional<int>&&, decltype(return_optional<int>)>);
static_assert(!has_or_else<std::optional<MoveOnly>&, decltype(return_optional<MoveOnly>)>);
static_assert(has_or_else<std::optional<MoveOnly>&&, decltype(return_optional<MoveOnly>)>);
static_assert(!has_or_else<std::optional<NonMovable>&, decltype(return_optional<NonMovable>)>);
static_assert(!has_or_else<std::optional<NonMovable>&&, decltype(return_optional<NonMovable>)>);

std::optional<int> take_int(int) { return 0; }
void take_int_return_void(int) {}

static_assert(!has_or_else<std::optional<int>, decltype(take_int)>);
static_assert(!has_or_else<std::optional<int>, decltype(take_int_return_void)>);
static_assert(!has_or_else<std::optional<int>, int>);

constexpr bool test() {
  {
    std::optional<int> opt;
    TC_ASSERT_EXPR(opt.or_else([] { return std::optional<int>{0}; }) == 0);
    opt = 1;
    opt.or_else([] {
      TC_ASSERT_EXPR(false);
      return std::optional<int>{};
    });
  }
  {
    std::optional<MoveOnly> opt;
    opt = std::move(opt).or_else([] { return std::optional<MoveOnly>{MoveOnly{}}; });
    std::move(opt).or_else([] {
      TC_ASSERT_EXPR(false);
      return std::optional<MoveOnly>{};
    });
  }

  return true;
}

int tc_utilities_optional_optional_monadic_or_else(void) {
  test();
  static_assert(test());
  return 0;
}
