//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <optional>

// [optional.relops], relational operators

// template<class T, three_way_comparable_with<T> U>
//   constexpr compare_three_way_result_t<T, U>
//     operator<=>(const optional<T>&, const optional<U>&);

#include <cassert>
#include <compare>
#include <optional>

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

constexpr bool test() {
  {
    std::optional<int> op1;
    std::optional<int> op2;

    TC_ASSERT_EXPR((op1 <=> op2) == std::strong_ordering::equal);
    TC_ASSERT_EXPR(testOrder(op1, op2, std::strong_ordering::equal));
  }
  {
    std::optional<int> op1{3};
    std::optional<int> op2{3};
    TC_ASSERT_EXPR((op1 <=> op1) == std::strong_ordering::equal);
    TC_ASSERT_EXPR(testOrder(op1, op1, std::strong_ordering::equal));
    TC_ASSERT_EXPR((op1 <=> op2) == std::strong_ordering::equal);
    TC_ASSERT_EXPR(testOrder(op1, op2, std::strong_ordering::equal));
    TC_ASSERT_EXPR((op2 <=> op1) == std::strong_ordering::equal);
    TC_ASSERT_EXPR(testOrder(op2, op1, std::strong_ordering::equal));
  }
  {
    std::optional<int> op;
    std::optional<int> op1{2};
    std::optional<int> op2{3};
    TC_ASSERT_EXPR((op <=> op2) == std::strong_ordering::less);
    TC_ASSERT_EXPR(testOrder(op, op2, std::strong_ordering::less));
    TC_ASSERT_EXPR((op1 <=> op2) == std::strong_ordering::less);
    TC_ASSERT_EXPR(testOrder(op1, op2, std::strong_ordering::less));
  }
  {
    std::optional<int> op;
    std::optional<int> op1{3};
    std::optional<int> op2{2};
    TC_ASSERT_EXPR((op1 <=> op) == std::strong_ordering::greater);
    TC_ASSERT_EXPR(testOrder(op1, op, std::strong_ordering::greater));
    TC_ASSERT_EXPR((op1 <=> op2) == std::strong_ordering::greater);
    TC_ASSERT_EXPR(testOrder(op1, op2, std::strong_ordering::greater));
  }

  return true;
}

#if TEST_STD_VER >= 20
TEST_CONSTEXPR_CXX20 bool test_full() {
  test();
  static_assert(test());
  return true;
}

int tc_utilities_optional_optional_relops_compare_three_way_pass(void) {
    test_full();
#if TEST_STD_VER > 20
    static_assert(test_full());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 20 */
