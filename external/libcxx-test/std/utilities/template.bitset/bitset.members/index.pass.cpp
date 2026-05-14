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
// bitset<N>::reference operator[](size_t pos); // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>
#include <vector>

#include "../bitset_test_cases.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_index() {
    std::vector<std::bitset<N> > const cases = get_test_cases<N>();
    for (std::size_t c = 0; c != cases.size(); ++c) {
        std::bitset<N> v1 = cases[c];
        if (v1.size() > 0) {
            TC_ASSERT_EXPR(v1[N/2] == v1.test(N/2));
            typename std::bitset<N>::reference r = v1[N/2];
            TC_ASSERT_EXPR(r == v1.test(N/2));
            typename std::bitset<N>::reference r2 = v1[N/2];
            r = r2;
            TC_ASSERT_EXPR(r == v1.test(N/2));
            r = false;
            TC_ASSERT_EXPR(r == false);
            TC_ASSERT_EXPR(v1.test(N/2) == false);
            r = true;
            TC_ASSERT_EXPR(r == true);
            TC_ASSERT_EXPR(v1.test(N/2) == true);
            bool b = ~r;
            TC_ASSERT_EXPR(r == true);
            TC_ASSERT_EXPR(v1.test(N/2) == true);
            TC_ASSERT_EXPR(b == false);
            r.flip();
            TC_ASSERT_EXPR(r == false);
            TC_ASSERT_EXPR(v1.test(N/2) == false);
        }
        ASSERT_SAME_TYPE(decltype(v1[0]), typename std::bitset<N>::reference);
    }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_index<0>();
  test_index<1>();
  test_index<31>();
  test_index<32>();
  test_index<33>();
  test_index<63>();
  test_index<64>();
  test_index<65>();

  std::bitset<1> set;
  set[0] = false;
  auto b = set[0];
  set[0] = true;
  TC_ASSERT_EXPR(b);

  return true;
}

int tc_utilities_template_bitset_bitset_members_index(void) {
  test();
  test_index<1000>(); // not in constexpr because of constexpr evaluation step limits
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
