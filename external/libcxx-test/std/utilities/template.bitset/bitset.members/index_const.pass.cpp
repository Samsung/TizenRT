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
// constexpr bool operator[](size_t pos) const; // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>
#include <vector>

#include "../bitset_test_cases.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_index_const() {
    std::vector<std::bitset<N> > const cases = get_test_cases<N>();
    for (std::size_t c = 0; c != cases.size(); ++c) {
        std::bitset<N> const v = cases[c];
        if (v.size() > 0) {
            TC_ASSERT_EXPR(v[N/2] == v.test(N/2));
        }
    }
#if !defined(_LIBCPP_VERSION) || defined(_LIBCPP_ABI_BITSET_VECTOR_BOOL_CONST_SUBSCRIPT_RETURN_BOOL)
    ASSERT_SAME_TYPE(decltype(cases[0][0]), bool);
#else
    ASSERT_SAME_TYPE(decltype(cases[0][0]), typename std::bitset<N>::const_reference);
#endif
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_index_const<0>();
  test_index_const<1>();
  test_index_const<31>();
  test_index_const<32>();
  test_index_const<33>();
  test_index_const<63>();
  test_index_const<64>();
  test_index_const<65>();

  std::bitset<1> set_;
  set_[0] = false;
  const auto& set = set_;
  auto b = set[0];
  set_[0] = true;
#if !defined(_LIBCPP_VERSION) || defined(_LIBCPP_ABI_BITSET_VECTOR_BOOL_CONST_SUBSCRIPT_RETURN_BOOL)
  TC_ASSERT_EXPR(!b);
#else
  TC_ASSERT_EXPR(b);
#endif

  return true;
}

int tc_utilities_template_bitset_bitset_members_index_const(void) {
  test();
  test_index_const<1000>(); // not in constexpr because of constexpr evaluation step limits
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
