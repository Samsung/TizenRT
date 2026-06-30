//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03

// <algorithm>

// template<class T, class Compare>
//   T
//   min(initializer_list<T> t, Compare comp);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_algorithms_alg_sorting_alg_min_max_min_init_list_comp(void) {
    int i = std::min({2, 3, 1}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({2, 1, 3}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({3, 1, 2}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({3, 2, 1}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({1, 2, 3}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({1, 3, 2}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
#if TEST_STD_VER >= 14
    {
    static_assert(std::min({1, 3, 2}, std::greater<int>()) == 3, "");
    static_assert(std::min({2, 1, 3}, std::greater<int>()) == 3, "");
    static_assert(std::min({3, 2, 1}, std::greater<int>()) == 3, "");
    }
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
