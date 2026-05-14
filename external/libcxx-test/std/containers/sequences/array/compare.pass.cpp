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
// <array>

// template <class T, size_t N>
//   bool operator==(const array<T,N>& x, const array<T,N>& y);    // constexpr in C++20
// template <class T, size_t N>
//   bool operator!=(const array<T,N>& x, const array<T,N>& y);    // removed in C++20
// template <class T, size_t N>
//   bool operator<(const array<T,N>& x, const array<T,N>& y);     // removed in C++20
// template <class T, size_t N>
//   bool operator>(const array<T,N>& x, const array<T,N>& y);     // removed in C++20
// template <class T, size_t N>
//   bool operator<=(const array<T,N>& x, const array<T,N>& y);    // removed in C++20
// template <class T, size_t N>
//   bool operator>=(const array<T,N>& x, const array<T,N>& y);    // removed in C++20

#include <array>
#include <cassert>

#include "test_macros.h"
#include "test_comparisons.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests() {
  // Arrays where the elements support all comparison operators
  AssertComparisonsReturnBool<std::array<int, 3> >();
  {
    typedef std::array<int, 3> C;
    const C c1 = {1, 2, 3};
    const C c2 = {1, 2, 3};
    const C c3 = {3, 2, 1};
    const C c4 = {1, 2, 1};
    TC_ASSERT_EXPR(testComparisons(c1, c2, true, false));
    TC_ASSERT_EXPR(testComparisons(c1, c3, false, true));
    TC_ASSERT_EXPR(testComparisons(c1, c4, false, false));
  }
  // Empty array
  {
    typedef std::array<int, 0> C;
    const C c1 = {};
    const C c2 = {};
    TC_ASSERT_EXPR(testComparisons(c1, c2, true, false));
  }
  // Arrays where the elements support only less and equality comparisons
  AssertComparisonsReturnBool<std::array<LessAndEqComp, 3> >();
  {
    typedef std::array<LessAndEqComp, 3> C;
    const C c1 = {LessAndEqComp(1), LessAndEqComp(2), LessAndEqComp(3)};
    const C c2 = {LessAndEqComp(1), LessAndEqComp(2), LessAndEqComp(3)};
    const C c3 = {LessAndEqComp(3), LessAndEqComp(2), LessAndEqComp(1)};
    const C c4 = {LessAndEqComp(1), LessAndEqComp(2), LessAndEqComp(1)};
    TC_ASSERT_EXPR(testComparisons(c1, c2, true, false));
    TC_ASSERT_EXPR(testComparisons(c1, c3, false, true));
    TC_ASSERT_EXPR(testComparisons(c1, c4, false, false));
  }
  // Empty array where the elements support only less and equality comparisons
  {
    typedef std::array<LessAndEqComp, 0> C;
    const C c1 = {};
    const C c2 = {};
    TC_ASSERT_EXPR(testComparisons(c1, c2, true, false));
  }

  return true;
}

int tc_containers_sequences_array_compare(void) {
  tests();
#if TEST_STD_VER >= 20
  static_assert(tests());
#endif
  return 0;
}
