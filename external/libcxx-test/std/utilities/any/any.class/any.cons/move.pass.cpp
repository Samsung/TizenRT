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

// any(any &&) noexcept;

#include <any>
#include <utility>
#include <type_traits>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// Moves are always noexcept. The throws_on_move object
// must be stored dynamically so the pointer is moved and
// not the stored object.
void test_move_does_not_throw()
{
#if !defined(TEST_HAS_NO_EXCEPTIONS)
    TC_ASSERT_EXPR(throws_on_move::count == 0);
    {
        throws_on_move v(42);
        std::any a = v;
        TC_ASSERT_EXPR(throws_on_move::count == 2);
        // No allocations should be performed after this point.
        DisableAllocationGuard g; ((void)g);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            const std::any a2 = std::move(a);
            assertEmpty(a);
            assertContains<throws_on_move>(a2, 42);
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(throws_on_move::count == 1);
        assertEmpty(a);
    }
    TC_ASSERT_EXPR(throws_on_move::count == 0);
#endif
}

void test_move_empty() {
    DisableAllocationGuard g; ((void)g); // no allocations should be performed.

    std::any a1;
    std::any a2 = std::move(a1);

    assertEmpty(a1);
    assertEmpty(a2);
}

template <class Type>
void test_move() {
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a = Type(42);
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 1);

        // Moving should not perform allocations since it must be noexcept.
        DisableAllocationGuard g; ((void)g);

        std::any a2 = std::move(a);

        TC_ASSERT_EXPR(Type::moved == 1 || Type::moved == 2); // zero or more move operations can be performed.
        TC_ASSERT_EXPR(Type::copied == 0); // no copies can be performed.
        TC_ASSERT_EXPR(Type::count == 1 + a.has_value());
        assertContains<Type>(a2, 42);
        LIBCPP_ASSERT(!a.has_value()); // Moves are always destructive.
        if (a.has_value())
            assertContains<Type>(a, 0);
    }
    TC_ASSERT_EXPR(Type::count == 0);
}

int tc_utilities_any_any_class_any_cons_move(void) {
    // noexcept test
    static_assert(std::is_nothrow_move_constructible<std::any>::value);

    test_move<small>();
    test_move<large>();
    test_move_empty();
    test_move_does_not_throw();

  return 0;
}
