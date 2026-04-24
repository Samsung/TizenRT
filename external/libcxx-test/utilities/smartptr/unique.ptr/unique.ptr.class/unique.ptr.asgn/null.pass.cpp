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
// <memory>

// unique_ptr

// Test unique_ptr move assignment

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

// test assignment from null
template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<IsArray, A[], A>::type VT;
  const int expect_alive = IsArray ? 5 : 1;
  {
    std::unique_ptr<VT> s2(newValue<VT>(expect_alive));
    if (!TEST_IS_CONSTANT_EVALUATED)
      TC_ASSERT_EXPR(A::count == expect_alive);
    s2 = NULL;
    if (!TEST_IS_CONSTANT_EVALUATED)
      TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(s2.get() == 0);
  }
  if (!TEST_IS_CONSTANT_EVALUATED)
    TC_ASSERT_EXPR(A::count == 0);
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic</*IsArray*/ false>();
  test_basic<true>();

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_asgn_null(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
