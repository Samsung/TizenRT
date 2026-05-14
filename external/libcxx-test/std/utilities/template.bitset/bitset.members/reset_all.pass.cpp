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
// bitset<N>& reset(); // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_reset_all() {
    std::bitset<N> v;
    v.set();
    v.reset();
    for (std::size_t i = 0; i < v.size(); ++i)
        TC_ASSERT_EXPR(!v[i]);
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_reset_all<0>();
  test_reset_all<1>();
  test_reset_all<31>();
  test_reset_all<32>();
  test_reset_all<33>();
  test_reset_all<63>();
  test_reset_all<64>();
  test_reset_all<65>();
  test_reset_all<1000>();

  return true;
}

int tc_utilities_template_bitset_bitset_members_reset_all(void) {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
