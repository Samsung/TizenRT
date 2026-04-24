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

// test get

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "unique_ptr_test_helper.h"
#include "libcxx_tc_common.h"

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<IsArray, int[], int>::type VT;
  typedef const VT CVT;
  {
    typedef std::unique_ptr<VT> U;
    int* p = newValue<VT>(1);
    U s(p);
    U const& sc = s;
    ASSERT_SAME_TYPE(decltype(s.get()), int*);
    ASSERT_SAME_TYPE(decltype(sc.get()), int*);
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(sc.get() == s.get());
  }
  {
    typedef std::unique_ptr<CVT> U;
    const int* p = newValue<VT>(1);
    U s(p);
    U const& sc = s;
    ASSERT_SAME_TYPE(decltype(s.get()), const int*);
    ASSERT_SAME_TYPE(decltype(sc.get()), const int*);
    TC_ASSERT_EXPR(s.get() == p);
    TC_ASSERT_EXPR(sc.get() == s.get());
  }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic</*IsArray*/ false>();
  test_basic<true>();

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_get(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
