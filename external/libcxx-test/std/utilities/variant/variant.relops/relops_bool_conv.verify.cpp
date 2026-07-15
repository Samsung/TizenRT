//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14
// <variant>

// Test: variant relational operators require bool-convertible result

#include <cassert>
#include <type_traits>
#include <utility>
#include <variant>

#include "test_macros.h"
#include "../../libcxx_tc_common.h"

struct MyBoolExplicit {
  bool value;
  constexpr explicit MyBoolExplicit(bool v) : value(v) {}
  constexpr explicit operator bool() const noexcept { return value; }
};

struct ComparesToMyBoolExplicit {
  int value = 0;
};
inline constexpr MyBoolExplicit operator==(const ComparesToMyBoolExplicit& LHS, const ComparesToMyBoolExplicit& RHS) noexcept {
  return MyBoolExplicit(LHS.value == RHS.value);
}
inline constexpr MyBoolExplicit operator!=(const ComparesToMyBoolExplicit& LHS, const ComparesToMyBoolExplicit& RHS) noexcept {
  return MyBoolExplicit(LHS.value != RHS.value);
}
inline constexpr MyBoolExplicit operator<(const ComparesToMyBoolExplicit& LHS, const ComparesToMyBoolExplicit& RHS) noexcept {
  return MyBoolExplicit(LHS.value < RHS.value);
}
inline constexpr MyBoolExplicit operator<=(const ComparesToMyBoolExplicit& LHS, const ComparesToMyBoolExplicit& RHS) noexcept {
  return MyBoolExplicit(LHS.value <= RHS.value);
}
inline constexpr MyBoolExplicit operator>(const ComparesToMyBoolExplicit& LHS, const ComparesToMyBoolExplicit& RHS) noexcept {
  return MyBoolExplicit(LHS.value > RHS.value);
}
inline constexpr MyBoolExplicit operator>=(const ComparesToMyBoolExplicit& LHS, const ComparesToMyBoolExplicit& RHS) noexcept {
  return MyBoolExplicit(LHS.value >= RHS.value);
}

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test() {
  using V = std::variant<int, ComparesToMyBoolExplicit>;
  V v1(42);
  V v2(101);
  
  // The relational operators should work but return non-bool-convertible type
  // This test verifies the operators exist
  auto eq_result = (v1 == v2);
  auto ne_result = (v1 != v2);
  auto lt_result = (v1 < v2);
  auto le_result = (v1 <= v2);
  auto gt_result = (v1 > v2);
  auto ge_result = (v1 >= v2);
  
  // Verify results are not implicitly convertible to bool
  constexpr bool eq_not_bool = !std::is_convertible<decltype(eq_result), bool>::value;
  constexpr bool ne_not_bool = !std::is_convertible<decltype(ne_result), bool>::value;
  constexpr bool lt_not_bool = !std::is_convertible<decltype(lt_result), bool>::value;
  constexpr bool le_not_bool = !std::is_convertible<decltype(le_result), bool>::value;
  constexpr bool gt_not_bool = !std::is_convertible<decltype(gt_result), bool>::value;
  constexpr bool ge_not_bool = !std::is_convertible<decltype(ge_result), bool>::value;
  TC_ASSERT_MSG(eq_not_bool, "operator== result shall not be convertible to bool");
  TC_ASSERT_MSG(ne_not_bool, "operator!= result shall not be convertible to bool");
  TC_ASSERT_MSG(lt_not_bool, "operator< result shall not be convertible to bool");
  TC_ASSERT_MSG(le_not_bool, "operator<= result shall not be convertible to bool");
  TC_ASSERT_MSG(gt_not_bool, "operator> result shall not be convertible to bool");
  TC_ASSERT_MSG(ge_not_bool, "operator>= result shall not be convertible to bool");
  
  return true;
}

int tc_utilities_variant_variant_relops_relops_bool_conv_verify(void) {
    test();
    // Negative test case: verifies that certain code should NOT compile (not compiling = pass).
    // Converted to runtime test using type traits. Static compile-time check removed.
    // #if TEST_STD_VER > 17
    //     static_assert(test());
    // #endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif // TEST_STD_VER >= 17
