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
// unsigned long to_ulong() const; // constexpr since C++23

#include <bitset>
#include <algorithm>
#include <type_traits>
#include <limits>
#include <climits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_to_ulong() {
    const std::size_t M = sizeof(unsigned long) * CHAR_BIT < N ? sizeof(unsigned long) * CHAR_BIT : N;
    const bool is_M_zero = std::integral_constant<bool, M == 0>::value; // avoid compiler warnings
    const std::size_t X = is_M_zero ? sizeof(unsigned long) * CHAR_BIT - 1 : sizeof(unsigned long) * CHAR_BIT - M;
    const std::size_t max = is_M_zero ? 0 : std::size_t(std::numeric_limits<unsigned long>::max()) >> X;
    std::size_t tests[] = {
        0,
        std::min<std::size_t>(1, max),
        std::min<std::size_t>(2, max),
        std::min<std::size_t>(3, max),
        std::min(max, max-3),
        std::min(max, max-2),
        std::min(max, max-1),
        max
    };
    for (std::size_t j : tests) {
        std::bitset<N> v(j);
        TC_ASSERT_EXPR(j == v.to_ulong());
    }

    { // test values bigger than can fit into the bitset
        const unsigned long val = 0x5AFFFFA5UL;
        const bool canFit = N < sizeof(unsigned long) * CHAR_BIT;
        const unsigned long mask = canFit ? (1UL << (canFit ? N : 0)) - 1 : (unsigned long)(-1); // avoid compiler warnings
        std::bitset<N> v(val);
        TC_ASSERT_EXPR(v.to_ulong() == (val & mask)); // we shouldn't return bit patterns from outside the limits of the bitset.
    }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_to_ulong<0>();
  test_to_ulong<1>();
  test_to_ulong<31>();
  test_to_ulong<32>();
  test_to_ulong<33>();
  test_to_ulong<63>();
  test_to_ulong<64>();
  test_to_ulong<65>();
  test_to_ulong<1000>();

  return true;
}

int tc_utilities_template_bitset_bitset_members_to_ulong(void) {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
