//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// bool none() const; // constexpr since C++23

#include <bitset>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_none() {
    std::bitset<N> v;
    v.reset();
    TC_ASSERT_EXPR(v.none() == true);
    v.set();
    TC_ASSERT_EXPR(v.none() == (N == 0));
    if (v.size() > 1) {
        v[N/2] = false;
        TC_ASSERT_EXPR(v.none() == false);
        v.reset();
        v[N/2] = true;
        TC_ASSERT_EXPR(v.none() == false);
    }
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_none<0>();
  test_none<1>();
  test_none<31>();
  test_none<32>();
  test_none<33>();
  test_none<63>();
  test_none<64>();
  test_none<65>();
  test_none<1000>();

  return true;
}

int tc_utilities_template_bitset_bitset_members_none(void) {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
