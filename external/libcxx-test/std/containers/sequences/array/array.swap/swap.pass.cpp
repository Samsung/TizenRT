//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <array>

// void swap(array& a);
// namespace std { void swap(array<T, N> &x, array<T, N> &y);

#include <cassert>
#include <array>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NonSwappable {
    TEST_CONSTEXPR NonSwappable() { }
private:
    NonSwappable(NonSwappable const&);
    NonSwappable& operator=(NonSwappable const&);
};

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C c1 = {1, 2, 3.5};
        C c2 = {4, 5, 6.5};
        c1.swap(c2);
        TC_ASSERT_EXPR(c1.size() == 3);
        TC_ASSERT_EXPR(c1[0] == 4);
        TC_ASSERT_EXPR(c1[1] == 5);
        TC_ASSERT_EXPR(c1[2] == 6.5);
        TC_ASSERT_EXPR(c2.size() == 3);
        TC_ASSERT_EXPR(c2[0] == 1);
        TC_ASSERT_EXPR(c2[1] == 2);
        TC_ASSERT_EXPR(c2[2] == 3.5);
    }
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C c1 = {1, 2, 3.5};
        C c2 = {4, 5, 6.5};
        std::swap(c1, c2);
        TC_ASSERT_EXPR(c1.size() == 3);
        TC_ASSERT_EXPR(c1[0] == 4);
        TC_ASSERT_EXPR(c1[1] == 5);
        TC_ASSERT_EXPR(c1[2] == 6.5);
        TC_ASSERT_EXPR(c2.size() == 3);
        TC_ASSERT_EXPR(c2[0] == 1);
        TC_ASSERT_EXPR(c2[1] == 2);
        TC_ASSERT_EXPR(c2[2] == 3.5);
    }

    {
        typedef double T;
        typedef std::array<T, 0> C;
        C c1 = {};
        C c2 = {};
        c1.swap(c2);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        C c1 = {};
        C c2 = {};
        std::swap(c1, c2);
        TC_ASSERT_EXPR(c1.size() == 0);
        TC_ASSERT_EXPR(c2.size() == 0);
    }
    {
        typedef NonSwappable T;
        typedef std::array<T, 0> C0;
        C0 l = {};
        C0 r = {};
        l.swap(r);
#if TEST_STD_VER >= 11
        static_assert(noexcept(l.swap(r)), "");
#endif
    }

    return true;
}

int tc_containers_sequences_array_array_swap_swap(void) {
    tests();
#if TEST_STD_VER >= 20
    static_assert(tests(), "");
#endif
  return 0;
}
