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
// <functional>
//
// reference_wrapper
//
// template <class U>
//   reference_wrapper(U&&);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct B {};

struct A1 {
  mutable B b_;
  TEST_CONSTEXPR operator B&() const { return b_; }
};

struct A2 {
  mutable B b_;
  TEST_CONSTEXPR operator B&() const TEST_NOEXCEPT { return b_; }
};

void implicitly_convert(std::reference_wrapper<B>) TEST_NOEXCEPT;

TEST_CONSTEXPR_CXX20 bool test()
{
  {
    A1 a;
    ASSERT_NOT_NOEXCEPT(implicitly_convert(a));
    std::reference_wrapper<B> b1 = a;
    TC_ASSERT_EXPR(&b1.get() == &a.b_);
    ASSERT_NOT_NOEXCEPT(b1 = a);
    b1 = a;
    TC_ASSERT_EXPR(&b1.get() == &a.b_);
  }
  {
    A2 a;
    ASSERT_NOEXCEPT(implicitly_convert(a));
    std::reference_wrapper<B> b2 = a;
    TC_ASSERT_EXPR(&b2.get() == &a.b_);
    ASSERT_NOEXCEPT(b2 = a);
    b2 = a;
    TC_ASSERT_EXPR(&b2.get() == &a.b_);
  }
  return true;
}

int tc_utilities_function_objects_refwrap_refwrap_const_type_conv_ctor2(void) {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return 0;
}
