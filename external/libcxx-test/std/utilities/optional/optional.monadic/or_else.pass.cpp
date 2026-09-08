//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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

#if TEST_STD_VER >= 23
TEST_CONSTEXPR_CXX20 bool test_full() {
  test();
  static_assert(test());
  return true;
}

int tc_utilities_optional_optional_monadic_or_else_pass(void) {
    test_full();
#if TEST_STD_VER > 23
    static_assert(test_full());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 23 */
