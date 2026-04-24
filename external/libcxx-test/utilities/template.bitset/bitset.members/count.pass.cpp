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
// size_t count() const; // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>
#include <vector>

#include "../bitset_test_cases.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_count() {
    std::vector<std::bitset<N> > const cases = get_test_cases<N>();
    for (std::size_t c = 0; c != cases.size(); ++c) {
        const std::bitset<N> v = cases[c];
        std::size_t c1 = v.count();
        std::size_t c2 = 0;
        for (std::size_t i = 0; i < v.size(); ++i)
            if (v[i])
                ++c2;
        TC_ASSERT_EXPR(c1 == c2);
    }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_count<0>();
  test_count<1>();
  test_count<31>();
  test_count<32>();
  test_count<33>();
  test_count<63>();
  test_count<64>();
  test_count<65>();

  return true;
}

int tc_utilities_template_bitset_bitset_members_count(void) {
  test();
  test_count<1000>(); // not in constexpr because of constexpr evaluation step limits
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
