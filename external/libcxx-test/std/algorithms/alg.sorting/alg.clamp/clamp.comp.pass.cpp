//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>
// XFAIL: c++03, c++11, c++14

// template<class T, class Compare>
//   const T&
//   clamp(const T& v, const T& lo, const T& hi, Compare comp);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace {
struct Tag {
    Tag() : val(0), tag("Default") {}
    Tag(int a, const char *b) : val(a), tag(b) {}
    ~Tag() {}

    int val;
    const char *tag;
    };

bool eq(const Tag& rhs, const Tag& lhs) { return rhs.val == lhs.val && rhs.tag == lhs.tag; }
// bool operator==(const Tag& rhs, const Tag& lhs) { return rhs.val == lhs.val; }
bool comp (const Tag& rhs, const Tag& lhs) { return rhs.val <  lhs.val; }


template <class T, class C>
void
test(const T& v, const T& lo, const T& hi, C c, const T& x)
{
    TC_ASSERT_EXPR(&std::clamp(v, lo, hi, c) == &x);
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_clamp_clamp_comp(void) {
    {
    int x = 0;
    int y = 0;
    int z = 0;
    test(x, y, z, std::greater<int>(), x);
    test(y, x, z, std::greater<int>(), y);
    }
    {
    int x = 0;
    int y = 1;
    int z = -1;
    test(x, y, z, std::greater<int>(), x);
    test(y, x, z, std::greater<int>(), x);
    }
    {
    int x = 1;
    int y = 0;
    int z = 0;
    test(x, y, z, std::greater<int>(), y);
    test(y, x, z, std::greater<int>(), y);
    }

    {
//  If they're all the same, we should get the value back.
    Tag x{0, "Zero-x"};
    Tag y{0, "Zero-y"};
    Tag z{0, "Zero-z"};
    TC_ASSERT_EXPR(eq(std::clamp(x, y, z, comp), x));
    TC_ASSERT_EXPR(eq(std::clamp(y, x, z, comp), y));
    }

    {
//  If it's the same as the lower bound, we get the value back.
    Tag x{0, "Zero-x"};
    Tag y{0, "Zero-y"};
    Tag z{1, "One-z"};
    TC_ASSERT_EXPR(eq(std::clamp(x, y, z, comp), x));
    TC_ASSERT_EXPR(eq(std::clamp(y, x, z, comp), y));
    }

    {
//  If it's the same as the upper bound, we get the value back.
    Tag x{1, "One-x"};
    Tag y{0, "Zero-y"};
    Tag z{1, "One-z"};
    TC_ASSERT_EXPR(eq(std::clamp(x, y, z, comp), x));
    TC_ASSERT_EXPR(eq(std::clamp(z, y, x, comp), z));
    }

    {
//  If the value is between, we should get the value back
    Tag x{1, "One-x"};
    Tag y{0, "Zero-y"};
    Tag z{2, "Two-z"};
    TC_ASSERT_EXPR(eq(std::clamp(x, y, z, comp), x));
    TC_ASSERT_EXPR(eq(std::clamp(y, x, z, comp), x));
    }

    {
//  If the value is less than the 'lo', we should get the lo back.
    Tag x{0, "Zero-x"};
    Tag y{1, "One-y"};
    Tag z{2, "Two-z"};
    TC_ASSERT_EXPR(eq(std::clamp(x, y, z, comp), y));
    TC_ASSERT_EXPR(eq(std::clamp(y, x, z, comp), y));
    }
    {
//  If the value is greater than 'hi', we should get hi back.
    Tag x{2, "Two-x"};
    Tag y{0, "Zero-y"};
    Tag z{1, "One-z"};
    TC_ASSERT_EXPR(eq(std::clamp(x, y, z, comp), z));
    TC_ASSERT_EXPR(eq(std::clamp(y, z, x, comp), z));
    }
  
#if TEST_STD_VER >= 20
    {
    typedef int T;
    constexpr T x = 1;
    constexpr T y = 0;
    constexpr T z = 0;
    static_assert(std::clamp(x, y, z, std::greater<T>()) == y, "" );
    static_assert(std::clamp(y, x, z, std::greater<T>()) == y, "" );
    }
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
