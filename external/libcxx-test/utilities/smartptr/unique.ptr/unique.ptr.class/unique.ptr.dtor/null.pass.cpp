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

// The deleter is not called if get() == 0

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class Deleter {
  int state_;

  Deleter(Deleter&);
  Deleter& operator=(Deleter&);

public:
  TEST_CONSTEXPR_CXX23 Deleter() : state_(0) {}

  TEST_CONSTEXPR_CXX23 int state() const { return state_; }

  TEST_CONSTEXPR_CXX23 void operator()(void*) { ++state_; }
};

template <class T>
TEST_CONSTEXPR_CXX23 void test_basic() {
  Deleter d;
  TC_ASSERT_EXPR(d.state() == 0);
  {
    std::unique_ptr<T, Deleter&> p(nullptr, d);
    TC_ASSERT_EXPR(p.get() == nullptr);
    TC_ASSERT_EXPR(&p.get_deleter() == &d);
  }
  TC_ASSERT_EXPR(d.state() == 0);
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_basic<int>();
  test_basic<int[]>();

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_class_unique_ptr_dtor_null(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
