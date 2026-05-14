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
// UNSUPPORTED: c++03, c++11
#include <memory>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

//    The only way to create an unique_ptr<T[]> is to default construct them.

class foo {
public:
  TEST_CONSTEXPR_CXX23 foo() : val_(3) {}
  TEST_CONSTEXPR_CXX23 int get() const { return val_; }

private:
  int val_;
};

TEST_CONSTEXPR_CXX23 bool test() {
  {
    auto p1 = std::make_unique<int[]>(5);
    for (int i = 0; i < 5; ++i)
      TC_ASSERT_EXPR(p1[i] == 0);
  }

  {
    auto p2 = std::make_unique<std::string[]>(5);
    for (int i = 0; i < 5; ++i)
      TC_ASSERT_EXPR(p2[i].size() == 0);
  }

  {
    auto p3 = std::make_unique<foo[]>(7);
    for (int i = 0; i < 7; ++i)
      TC_ASSERT_EXPR(p3[i].get() == 3);
  }

  return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_create_make_unique_array(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
