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

// test get_deleter()

#include <memory>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Deleter {
  TEST_CONSTEXPR_CXX23 Deleter() {}

  TEST_CONSTEXPR_CXX23 void operator()(void*) const {}

  TEST_CONSTEXPR_CXX23 int test() { return 5; }
  TEST_CONSTEXPR_CXX23 int test() const { return 6; }
};

template <bool IsArray>
TEST_CONSTEXPR_CXX23 void test_basic() {
  typedef typename std::conditional<IsArray, int[], int>::type VT;
  {
    std::unique_ptr<int, Deleter> p;
    TC_ASSERT_EXPR(p.get_deleter().test() == 5);
  }
  {
    const std::unique_ptr<VT, Deleter> p;
    TC_ASSERT_EXPR(p.get_deleter().test() == 6);
  }
  {
    typedef std::unique_ptr<VT, const Deleter&> UPtr;
    const Deleter d;
    UPtr p(nullptr, d);
    const UPtr& cp = p;
    ASSERT_SAME_TYPE(decltype(p.get_deleter()), const Deleter&);
    ASSERT_SAME_TYPE(decltype(cp.get_deleter()), const Deleter&);
    TC_ASSERT_EXPR(p.get_deleter().test() == 6);
    TC_ASSERT_EXPR(cp.get_deleter().test() == 6);
  }
  {
    typedef std::unique_ptr<VT, Deleter&> UPtr;
    Deleter d;
    UPtr p(nullptr, d);
    const UPtr& cp = p;
    ASSERT_SAME_TYPE(decltype(p.get_deleter()), Deleter&);
    ASSERT_SAME_TYPE(decltype(cp.get_deleter()), Deleter&);
    TC_ASSERT_EXPR(p.get_deleter().test() == 5);
    TC_ASSERT_EXPR(cp.get_deleter().test() == 5);
  }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic</*IsArray*/ false>();
  test_basic<true>();

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_observers_get_deleter(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
