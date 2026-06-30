//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-bad_any_cast-missing && !no-exceptions

// <any>

// any::swap(any &) noexcept

// Test swap(large, small) and swap(small, large)

#include <any>
#include <cassert>

#include "test_macros.h"
#include "any_helpers.h"
#include "libcxx_tc_common.h"

template <class LHS, class RHS>
void test_swap() {
    TC_ASSERT_EXPR(LHS::count == 0);
    TC_ASSERT_EXPR(RHS::count == 0);
    {
        std::any a1 = LHS(1);
        std::any a2 = RHS(2);
        TC_ASSERT_EXPR(LHS::count == 1);
        TC_ASSERT_EXPR(RHS::count == 1);

        a1.swap(a2);

        TC_ASSERT_EXPR(LHS::count == 1);
        TC_ASSERT_EXPR(RHS::count == 1);

        assertContains<RHS>(a1, 2);
        assertContains<LHS>(a2, 1);
    }
    TC_ASSERT_EXPR(LHS::count == 0);
    TC_ASSERT_EXPR(RHS::count == 0);
    TC_ASSERT_EXPR(LHS::copied == 0);
    TC_ASSERT_EXPR(RHS::copied == 0);
}

template <class Tp>
void test_swap_empty() {
    TC_ASSERT_EXPR(Tp::count == 0);
    {
        std::any a1 = Tp(1);
        std::any a2;
        TC_ASSERT_EXPR(Tp::count == 1);

        a1.swap(a2);

        TC_ASSERT_EXPR(Tp::count == 1);

        assertContains<Tp>(a2, 1);
        assertEmpty(a1);
    }
    TC_ASSERT_EXPR(Tp::count == 0);
    {
        std::any a1 = Tp(1);
        std::any a2;
        TC_ASSERT_EXPR(Tp::count == 1);

        a2.swap(a1);

        TC_ASSERT_EXPR(Tp::count == 1);

        assertContains<Tp>(a2, 1);
        assertEmpty(a1);
    }
    TC_ASSERT_EXPR(Tp::count == 0);
    TC_ASSERT_EXPR(Tp::copied == 0);
}

void test_noexcept()
{
    std::any a1;
    std::any a2;
    ASSERT_NOEXCEPT(a1.swap(a2));
}

void test_self_swap() {
    {
        // empty
        std::any a;
        a.swap(a);
        assertEmpty(a);
    }
    { // small
        using T = small;
        std::any a = T(42);
        T::reset();
        a.swap(a);
        assertContains<T>(a, 42);
        TC_ASSERT_EXPR(T::count == 1);
        TC_ASSERT_EXPR(T::copied == 0);
        LIBCPP_ASSERT(T::moved == 0);
    }
    TC_ASSERT_EXPR(small::count == 0);
    { // large
        using T = large;
        std::any a = T(42);
        T::reset();
        a.swap(a);
        assertContains<T>(a, 42);
        TC_ASSERT_EXPR(T::count == 1);
        TC_ASSERT_EXPR(T::copied == 0);
        LIBCPP_ASSERT(T::moved == 0);
    }
    TC_ASSERT_EXPR(large::count == 0);
}

int tc_utilities_any_any_class_any_modifiers_swap(void) {
    test_noexcept();
    test_swap_empty<small>();
    test_swap_empty<large>();
    test_swap<small1, small2>();
    test_swap<large1, large2>();
    test_swap<small, large>();
    test_swap<large, small>();
    test_self_swap();

  return 0;
}
