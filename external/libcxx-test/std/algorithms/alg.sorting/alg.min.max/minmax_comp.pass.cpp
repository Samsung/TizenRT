//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<class T, StrictWeakOrder<auto, T> Compare>
//   requires !SameType<T, Compare> && CopyConstructible<Compare>
//   pair<const T&, const T&>
//   minmax(const T& a, const T& b, Compare comp);

#include <algorithm>
#include <cassert>
#include <functional>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace {
template <class T, class C>
void
test(const T& a, const T& b, C c, const T& x, const T& y)
{
    std::pair<const T&, const T&> p = std::minmax(a, b, c);
    TC_ASSERT_EXPR(&p.first == &x);
    TC_ASSERT_EXPR(&p.second == &y);
}


} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_min_max_minmax_comp(void) {
    {
    int x = 0;
    int y = 0;
    test(x, y, std::greater<int>(), x, y);
    test(y, x, std::greater<int>(), y, x);
    }
    {
    int x = 0;
    int y = 1;
    test(x, y, std::greater<int>(), y, x);
    test(y, x, std::greater<int>(), y, x);
    }
    {
    int x = 1;
    int y = 0;
    test(x, y, std::greater<int>(), x, y);
    test(y, x, std::greater<int>(), x, y);
    }
#if TEST_STD_VER >= 14
    {
//  Note that you can't take a reference to a local var, since
//  its address is not a compile-time constant.
    constexpr static int x = 1;
    constexpr static int y = 0;
    constexpr auto p1 = std::minmax(x, y, std::greater<>());
    static_assert(p1.first  == x, "");
    static_assert(p1.second == y, "");
    constexpr auto p2 = std::minmax(y, x, std::greater<>());
    static_assert(p2.first  == x, "");
    static_assert(p2.second == y, "");
    }
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
