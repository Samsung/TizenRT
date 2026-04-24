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

// test reset

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX23 bool test() {
  {
    std::unique_ptr<A> p(new A);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      TC_ASSERT_EXPR(A::count == 1);
      TC_ASSERT_EXPR(B::count == 0);
    }
    A* i = p.get();
    TC_ASSERT_EXPR(i != nullptr);
    p.reset(new B);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      TC_ASSERT_EXPR(A::count == 1);
      TC_ASSERT_EXPR(B::count == 1);
    }
  }
  if (!TEST_IS_CONSTANT_EVALUATED) {
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);
  }
  {
    std::unique_ptr<A> p(new B);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      TC_ASSERT_EXPR(A::count == 1);
      TC_ASSERT_EXPR(B::count == 1);
    }
    A* i = p.get();
    TC_ASSERT_EXPR(i != nullptr);
    p.reset(new B);
    if (!TEST_IS_CONSTANT_EVALUATED) {
      TC_ASSERT_EXPR(A::count == 1);
      TC_ASSERT_EXPR(B::count == 1);
    }
  }
  if (!TEST_IS_CONSTANT_EVALUATED) {
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);
  }

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_modifiers_reset_single(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
