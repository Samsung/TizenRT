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

// test constexpr explicit operator bool() const noexcept; // constexpr since C++23

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

template <class UPtr>
TEST_CONSTEXPR_CXX23 void doTest(UPtr& p, bool ExpectTrue) {
  if (p)
    TC_ASSERT_EXPR(ExpectTrue);
  else
    TC_ASSERT_EXPR(!ExpectTrue);

  if (!p)
    TC_ASSERT_EXPR(!ExpectTrue);
  else
    TC_ASSERT_EXPR(ExpectTrue);
}

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<IsArray, int[], int>::type VT;
  typedef std::unique_ptr<VT> U;
  {
    static_assert((std::is_constructible<bool, U>::value), "");
    static_assert((std::is_constructible<bool, U const&>::value), "");
  }
#if TEST_STD_VER >= 11
  {
    static_assert(!std::is_convertible<U, bool>::value, "");
    static_assert(!std::is_convertible<U const&, bool>::value, "");
  }
#endif
  {
    U p(newValue<VT>(1));
    U const& cp = p;
    doTest(p, true);
    doTest(cp, true);
  }
  {
    U p;
    const U& cp = p;
    doTest(p, false);
    doTest(cp, false);
  }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic</*IsArray*/ false>();
  test_basic<true>();

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_explicit_bool(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}