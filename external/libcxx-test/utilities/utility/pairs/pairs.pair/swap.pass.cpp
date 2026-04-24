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
// <utility>

// template <class T1, class T2> struct pair

// void swap(pair& p);

#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct S {
    int i;
    TEST_CONSTEXPR_CXX20 S() : i(0) {}
    TEST_CONSTEXPR_CXX20 S(int j) : i(j) {}
    TEST_CONSTEXPR_CXX20 bool operator==(int x) const { return i == x; }
};

TEST_CONSTEXPR_CXX20 bool test() {
  {
    typedef std::pair<int, short> P1;
    P1 p1(3, static_cast<short>(4));
    P1 p2(5, static_cast<short>(6));
    p1.swap(p2);
    TC_ASSERT_EXPR(p1.first == 5);
    TC_ASSERT_EXPR(p1.second == 6);
    TC_ASSERT_EXPR(p2.first == 3);
    TC_ASSERT_EXPR(p2.second == 4);
  }
  {
    typedef std::pair<int, S> P1;
    P1 p1(3, S(4));
    P1 p2(5, S(6));
    p1.swap(p2);
    TC_ASSERT_EXPR(p1.first == 5);
    TC_ASSERT_EXPR(p1.second == 6);
    TC_ASSERT_EXPR(p2.first == 3);
    TC_ASSERT_EXPR(p2.second == 4);
  }
  return true;
}

int tc_utilities_utility_pairs_pairs_pair_swap(void) {
  test();
#if TEST_STD_VER >= 20
  static_assert(test());
#endif

  return 0;
}
