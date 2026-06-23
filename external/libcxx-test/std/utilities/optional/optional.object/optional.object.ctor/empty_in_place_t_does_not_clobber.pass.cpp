//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// constexpr optional(in_place_t);

// Test that the SFINAE "is_constructible<value_type>" isn't evaluated by the
// in_place_t constructor with no arguments when the Clang is trying to check
// copy constructor.

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"

using std::optional;

struct Wrapped {
  struct Inner {
    bool Dummy = true;
  };
  std::optional<Inner> inner;
};

#if TEST_STD_VER >= 17
namespace {
TEST_CONSTEXPR_CXX20 bool test() {
  static_assert(std::is_default_constructible<Wrapped::Inner>::value, "");
  Wrapped w;
  w.inner.emplace();
  TC_ASSERT_EXPR(w.inner.has_value());
  return true;
}
} // namespace

int tc_utilities_optional_optional_object_optional_object_ctor_empty_in_place_t_does_not_clobber_pass(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 17 */
