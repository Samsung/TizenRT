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

// template <class ValueType>
// any& operator=(ValueType&&);

// Test value copy and move assignment.

#include <any>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class LHS, class RHS>
void test_assign_value() {
    TC_ASSERT_EXPR(LHS::count == 0);
    TC_ASSERT_EXPR(RHS::count == 0);
    LHS::reset();
    RHS::reset();
    {
        std::any lhs = LHS(1);
        const std::any rhs = RHS(2);

        TC_ASSERT_EXPR(LHS::count == 1);
        TC_ASSERT_EXPR(RHS::count == 1);
        TC_ASSERT_EXPR(RHS::copied == 0);

        lhs = rhs;

        TC_ASSERT_EXPR(RHS::copied == 1);
        TC_ASSERT_EXPR(LHS::count == 0);
        TC_ASSERT_EXPR(RHS::count == 2);

        assertContains<RHS>(lhs, 2);
        assertContains<RHS>(rhs, 2);
    }
    TC_ASSERT_EXPR(LHS::count == 0);
    TC_ASSERT_EXPR(RHS::count == 0);
    LHS::reset();
    RHS::reset();
    {
        std::any lhs = LHS(1);
        std::any rhs = RHS(2);

        TC_ASSERT_EXPR(LHS::count == 1);
        TC_ASSERT_EXPR(RHS::count == 1);
        TC_ASSERT_EXPR(RHS::moved == 1);

        lhs = std::move(rhs);

        TC_ASSERT_EXPR(RHS::moved >= 1);
        TC_ASSERT_EXPR(RHS::copied == 0);
        TC_ASSERT_EXPR(LHS::count == 0);
        TC_ASSERT_EXPR(RHS::count == 1 + rhs.has_value());
        LIBCPP_ASSERT(!rhs.has_value());

        assertContains<RHS>(lhs, 2);
        if (rhs.has_value())
            assertContains<RHS>(rhs, 0);
    }
    TC_ASSERT_EXPR(LHS::count == 0);
    TC_ASSERT_EXPR(RHS::count == 0);
}

template <class RHS>
void test_assign_value_empty() {
    TC_ASSERT_EXPR(RHS::count == 0);
    RHS::reset();
    {
        std::any lhs;
        RHS rhs(42);
        TC_ASSERT_EXPR(RHS::count == 1);
        TC_ASSERT_EXPR(RHS::copied == 0);

        lhs = rhs;

        TC_ASSERT_EXPR(RHS::count == 2);
        TC_ASSERT_EXPR(RHS::copied == 1);
        TC_ASSERT_EXPR(RHS::moved >= 0);
        assertContains<RHS>(lhs, 42);
    }
    TC_ASSERT_EXPR(RHS::count == 0);
    RHS::reset();
    {
        std::any lhs;
        RHS rhs(42);
        TC_ASSERT_EXPR(RHS::count == 1);
        TC_ASSERT_EXPR(RHS::moved == 0);

        lhs = std::move(rhs);

        TC_ASSERT_EXPR(RHS::count == 2);
        TC_ASSERT_EXPR(RHS::copied == 0);
        TC_ASSERT_EXPR(RHS::moved >= 1);
        assertContains<RHS>(lhs, 42);
    }
    TC_ASSERT_EXPR(RHS::count == 0);
    RHS::reset();
}


template <class Tp, bool Move = false>
void test_assign_throws() {
#if !defined(TEST_HAS_NO_EXCEPTIONS)
    auto try_throw =
    [](std::any& lhs, Tp& rhs) {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            Move ? lhs = std::move(rhs)
                 : lhs = rhs;
            TC_ASSERT_EXPR(false);
        } catch (const my_any_exception&) {
            // do nothing
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    };
    // const lvalue to empty
    {
        std::any lhs;
        Tp rhs(1);
        TC_ASSERT_EXPR(Tp::count == 1);

        try_throw(lhs, rhs);

        TC_ASSERT_EXPR(Tp::count == 1);
        assertEmpty<Tp>(lhs);
    }
    {
        std::any lhs = small(2);
        Tp rhs(1);
        TC_ASSERT_EXPR(small::count == 1);
        TC_ASSERT_EXPR(Tp::count == 1);

        try_throw(lhs, rhs);

        TC_ASSERT_EXPR(small::count == 1);
        TC_ASSERT_EXPR(Tp::count == 1);
        assertContains<small>(lhs, 2);
    }
    {
        std::any lhs = large(2);
        Tp rhs(1);
        TC_ASSERT_EXPR(large::count == 1);
        TC_ASSERT_EXPR(Tp::count == 1);

        try_throw(lhs, rhs);

        TC_ASSERT_EXPR(large::count == 1);
        TC_ASSERT_EXPR(Tp::count == 1);
        assertContains<large>(lhs, 2);
    }
#endif
}


// Test that any& operator=(ValueType&&) is *never* selected for:
// * std::in_place type.
// * Non-copyable types
void test_sfinae_constraints() {
    { // Only the constructors are required to SFINAE on in_place_t
        using Tag = std::in_place_type_t<int>;
        using RawTag = std::remove_reference_t<Tag>;
        static_assert(std::is_assignable<std::any, RawTag&&>::value, "");
    }
    {
        struct Dummy { Dummy() = delete; };
        using T = std::in_place_type_t<Dummy>;
        static_assert(std::is_assignable<std::any, T>::value, "");
    }
    {
        // Test that the ValueType&& constructor SFINAE's away when the
        // argument is non-copyable
        struct NoCopy {
          NoCopy() = default;
          NoCopy(NoCopy const&) = delete;
          NoCopy(NoCopy&&) = default;
        };
        static_assert(!std::is_assignable<std::any, NoCopy>::value, "");
        static_assert(!std::is_assignable<std::any, NoCopy&>::value, "");
    }
}

int tc_utilities_any_any_class_any_assign_value(void) {
    test_assign_value<small1, small2>();
    test_assign_value<large1, large2>();
    test_assign_value<small, large>();
    test_assign_value<large, small>();
    test_assign_value_empty<small>();
    test_assign_value_empty<large>();
    test_assign_throws<small_throws_on_copy>();
    test_assign_throws<large_throws_on_copy>();
    test_assign_throws<throws_on_move, /* Move = */ true>();
    test_sfinae_constraints();

  return 0;
}
