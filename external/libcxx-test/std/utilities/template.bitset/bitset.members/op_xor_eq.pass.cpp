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
// bitset<N>& operator^=(const bitset<N>& rhs); // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>
#include <vector>

#include "../bitset_test_cases.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 bool test_op_xor_eq() {
    std::vector<std::bitset<N> > const cases = get_test_cases<N>();
    for (std::size_t c1 = 0; c1 != cases.size(); ++c1) {
        for (std::size_t c2 = 0; c2 != cases.size(); ++c2) {
            std::bitset<N> v1 = cases[c1];
            std::bitset<N> v2 = cases[c2];
            std::bitset<N> v3 = v1;
            v1 ^= v2;
            for (std::size_t i = 0; i < v1.size(); ++i)
                TC_ASSERT_EXPR(v1[i] == (v3[i] != v2[i]));
        }
    }
    return true;
}

int tc_utilities_template_bitset_bitset_members_op_xor_eq(void) {
  test_op_xor_eq<0>();
  test_op_xor_eq<1>();
  test_op_xor_eq<31>();
  test_op_xor_eq<32>();
  test_op_xor_eq<33>();
  test_op_xor_eq<63>();
  test_op_xor_eq<64>();
  test_op_xor_eq<65>();
  test_op_xor_eq<1000>(); // not in constexpr because of constexpr evaluation step limits
#if TEST_STD_VER > 20
  static_assert(test_op_xor_eq<0>());
  static_assert(test_op_xor_eq<1>());
  static_assert(test_op_xor_eq<31>());
  static_assert(test_op_xor_eq<32>());
  static_assert(test_op_xor_eq<33>());
  static_assert(test_op_xor_eq<63>());
  static_assert(test_op_xor_eq<64>());
  static_assert(test_op_xor_eq<65>());
#endif

  return 0;
}
