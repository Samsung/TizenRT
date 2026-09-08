//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// size_t count() const; // constexpr since C++23

#include <bitset>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <std::size_t N>
TEST_CONSTEXPR_CXX23 void test_size() {
    const std::bitset<N> v;
    TC_ASSERT_EXPR(v.size() == N);
}

TEST_CONSTEXPR_CXX23 bool test() {
  test_size<0>();
  test_size<1>();
  test_size<31>();
  test_size<32>();
  test_size<33>();
  test_size<63>();
  test_size<64>();
  test_size<65>();
  test_size<1000>();

  return true;
}

int tc_utilities_template_bitset_bitset_members_size(void) {
  test();
#if TEST_STD_VER > 20
  static_assert(test());
#endif

  return 0;
}
