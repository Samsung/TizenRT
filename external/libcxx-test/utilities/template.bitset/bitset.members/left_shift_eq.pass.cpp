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
// bitset<N>& operator<<=(size_t pos); // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>
#include <vector>

#include "../bitset_test_cases.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 bool test_left_shift() {
    std::vector<std::bitset<N> > const cases = get_test_cases<N>();
    for (std::size_t c = 0; c != cases.size(); ++c) {
        for (std::size_t s = 0; s <= N+1; ++s) {
            std::bitset<N> v1 = cases[c];
            std::bitset<N> v2 = v1;
            v1 <<= s;
            for (std::size_t i = 0; i < v1.size(); ++i)
                if (i < s)
                    TC_ASSERT_EXPR(v1[i] == 0);
                else
                    TC_ASSERT_EXPR(v1[i] == v2[i-s]);
        }
    }
    return true;
}

int tc_utilities_template_bitset_bitset_members_left_shift_eq(void) {
  test_left_shift<0>();
  test_left_shift<1>();
  test_left_shift<31>();
  test_left_shift<32>();
  test_left_shift<33>();
  test_left_shift<63>();
  test_left_shift<64>();
  test_left_shift<65>();
  test_left_shift<1000>(); // not in constexpr because of constexpr evaluation step limits
#if TEST_STD_VER > 20
  static_assert(test_left_shift<0>());
  static_assert(test_left_shift<1>());
  static_assert(test_left_shift<31>());
  static_assert(test_left_shift<32>());
  static_assert(test_left_shift<33>());
  static_assert(test_left_shift<63>());
  static_assert(test_left_shift<64>());
  static_assert(test_left_shift<65>());
#endif

  return 0;
}
