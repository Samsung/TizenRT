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
