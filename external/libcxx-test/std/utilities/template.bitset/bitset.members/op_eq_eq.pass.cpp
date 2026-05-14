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
// test:

// bool operator==(const bitset<N>& rhs) const; // constexpr since C++23
// bool operator!=(const bitset<N>& rhs) const; // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>
#include <vector>

#include "../bitset_test_cases.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_equality() {
    std::vector<std::bitset<N> > const cases = get_test_cases<N>();
    for (std::size_t c = 0; c != cases.size(); ++c) {
        std::bitset<N> const v1 = cases[c];
        std::bitset<N> v2 = v1;
        TC_ASSERT_EXPR(v1 == v2);
        if (v1.size() > 0) {
            v2[N/2].flip();
            TC_ASSERT_EXPR(v1 != v2);
        }
    }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_equality<0>();
  test_equality<1>();
  test_equality<31>();
  test_equality<32>();
  test_equality<33>();
  test_equality<63>();
  test_equality<64>();
  test_equality<65>();

  return true;
}

int tc_utilities_template_bitset_bitset_members_op_eq_eq(void) {
  test();
  test_equality<1000>(); // not in constexpr because of constexpr evaluation step limits
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
