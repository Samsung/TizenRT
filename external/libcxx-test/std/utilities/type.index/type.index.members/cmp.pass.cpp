//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <typeindex>

// class type_index

// bool operator==(const type_index& rhs) const noexcept;
// bool operator!=(const type_index& rhs) const noexcept;
// bool operator< (const type_index& rhs) const noexcept;
// bool operator<=(const type_index& rhs) const noexcept;
// bool operator> (const type_index& rhs) const noexcept;
// bool operator>=(const type_index& rhs) const noexcept;
// strong_ordering operator<=>(const type_index& rhs) const noexcept;

// UNSUPPORTED: no-rtti

#include <typeindex>
#include <cassert>

#include "test_macros.h"
#include "test_comparisons.h"
#include "libcxx_tc_common.h"

int tc_utilities_type_index_type_index_members_cmp(void) {
  AssertComparisonsAreNoexcept<std::type_index>();
  AssertComparisonsReturnBool<std::type_index>();
#if TEST_STD_VER > 17
  AssertOrderAreNoexcept<std::type_index>();
  AssertOrderReturn<std::strong_ordering, std::type_index>();
#endif

  std::type_index t1 = typeid(int);
  std::type_index t2 = typeid(int);
  std::type_index t3 = typeid(long);

  // Test `t1` and `t2` which should compare equal
  TC_ASSERT_EXPR(testComparisons(t1, t2, /*isEqual*/ true, /*isLess*/ false));
#if TEST_STD_VER > 17
  TC_ASSERT_EXPR(testOrder(t1, t2, std::strong_ordering::equal));
#endif

  // Test `t1` and `t3` which are not equal
  bool is_less = t1 < t3;
  TC_ASSERT_EXPR(testComparisons(t1, t3, /*isEqual*/ false, is_less));
#if TEST_STD_VER > 17
  TC_ASSERT_EXPR(testOrder(t1, t3, is_less ? std::strong_ordering::less : std::strong_ordering::greater));
#endif

  return 0;
}
