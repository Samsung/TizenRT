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

// template<class T>
//   pair<T, T>
//   minmax(initializer_list<T> t);

#include <algorithm>
#include <cassert>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_algorithms_alg_sorting_alg_min_max_minmax_init_list(void) {
    TC_ASSERT_EXPR((std::minmax({1, 2, 3}) == std::pair<int, int>(1, 3)));
    TC_ASSERT_EXPR((std::minmax({1, 3, 2}) == std::pair<int, int>(1, 3)));
    TC_ASSERT_EXPR((std::minmax({2, 1, 3}) == std::pair<int, int>(1, 3)));
    TC_ASSERT_EXPR((std::minmax({2, 3, 1}) == std::pair<int, int>(1, 3)));
    TC_ASSERT_EXPR((std::minmax({3, 1, 2}) == std::pair<int, int>(1, 3)));
    TC_ASSERT_EXPR((std::minmax({3, 2, 1}) == std::pair<int, int>(1, 3)));
#if TEST_STD_VER >= 14
    {
    static_assert((std::minmax({1, 2, 3}) == std::pair<int, int>(1, 3)), "");
    static_assert((std::minmax({1, 3, 2}) == std::pair<int, int>(1, 3)), "");
    static_assert((std::minmax({2, 1, 3}) == std::pair<int, int>(1, 3)), "");
    static_assert((std::minmax({2, 3, 1}) == std::pair<int, int>(1, 3)), "");
    static_assert((std::minmax({3, 1, 2}) == std::pair<int, int>(1, 3)), "");
    static_assert((std::minmax({3, 2, 1}) == std::pair<int, int>(1, 3)), "");
    }
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
