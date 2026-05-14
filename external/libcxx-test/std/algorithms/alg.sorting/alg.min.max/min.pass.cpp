//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<LessThanComparable T>
//   const T&
//   min(const T& a, const T& b);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace {
template <class T>
void
test(const T& a, const T& b, const T& x)
{
    TC_ASSERT_EXPR(&std::min(a, b) == &x);
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_min_max_min(void) {
    {
    int x = 0;
    int y = 0;
    test(x, y, x);
    test(y, x, y);
    }
    {
    int x = 0;
    int y = 1;
    test(x, y, x);
    test(y, x, x);
    }
    {
    int x = 1;
    int y = 0;
    test(x, y, y);
    test(y, x, y);
    }
#if TEST_STD_VER >= 14
    {
    constexpr int x = 1;
    constexpr int y = 0;
    static_assert(std::min(x, y) == y, "" );
    static_assert(std::min(y, x) == y, "" );
    }
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
