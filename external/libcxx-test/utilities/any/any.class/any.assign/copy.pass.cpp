/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-bad_any_cast-missing && !no-exceptions

// <any>

// any& operator=(const any&);

// Test copy assignment

#include <any>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class LHS, class RHS>
void test_copy_assign() {
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
}

template <class LHS>
void test_copy_assign_empty() {
    TC_ASSERT_EXPR(LHS::count == 0);
    LHS::reset();
    {
        std::any lhs;
        const std::any rhs = LHS(42);

        TC_ASSERT_EXPR(LHS::count == 1);
        TC_ASSERT_EXPR(LHS::copied == 0);

        lhs = rhs;

        TC_ASSERT_EXPR(LHS::copied == 1);
        TC_ASSERT_EXPR(LHS::count == 2);

        assertContains<LHS>(lhs, 42);
        assertContains<LHS>(rhs, 42);
    }
    TC_ASSERT_EXPR(LHS::count == 0);
    LHS::reset();
    {
        std::any lhs = LHS(1);
        const std::any rhs;

        TC_ASSERT_EXPR(LHS::count == 1);
        TC_ASSERT_EXPR(LHS::copied == 0);

        lhs = rhs;

        TC_ASSERT_EXPR(LHS::copied == 0);
        TC_ASSERT_EXPR(LHS::count == 0);

        assertEmpty<LHS>(lhs);
        assertEmpty(rhs);
    }
    TC_ASSERT_EXPR(LHS::count == 0);
}

void test_copy_assign_self() {
    // empty
    {
        std::any a;
        a = (std::any&)a;
        assertEmpty(a);
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    // small
    {
        std::any a = small(1);
        TC_ASSERT_EXPR(small::count == 1);

        a = (std::any&)a;

        TC_ASSERT_EXPR(small::count == 1);
        assertContains<small>(a, 1);
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    }
    TC_ASSERT_EXPR(small::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    // large
    {
        std::any a = large(1);
        TC_ASSERT_EXPR(large::count == 1);

        a = (std::any&)a;

        TC_ASSERT_EXPR(large::count == 1);
        assertContains<large>(a, 1);
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    }
    TC_ASSERT_EXPR(large::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
}

template <class Tp>
void test_copy_assign_throws()
{
#if !defined(TEST_HAS_NO_EXCEPTIONS)
    auto try_throw =
    [](std::any& lhs, const std::any& rhs) {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            lhs = rhs;
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
        const std::any rhs = Tp(1);
        TC_ASSERT_EXPR(Tp::count == 1);

        try_throw(lhs, rhs);

        TC_ASSERT_EXPR(Tp::count == 1);
        assertEmpty<Tp>(lhs);
        assertContains<Tp>(rhs, 1);
    }
    {
        std::any lhs = small(2);
        const std::any rhs = Tp(1);
        TC_ASSERT_EXPR(small::count == 1);
        TC_ASSERT_EXPR(Tp::count == 1);

        try_throw(lhs, rhs);

        TC_ASSERT_EXPR(small::count == 1);
        TC_ASSERT_EXPR(Tp::count == 1);
        assertContains<small>(lhs, 2);
        assertContains<Tp>(rhs, 1);
    }
    {
        std::any lhs = large(2);
        const std::any rhs = Tp(1);
        TC_ASSERT_EXPR(large::count == 1);
        TC_ASSERT_EXPR(Tp::count == 1);

        try_throw(lhs, rhs);

        TC_ASSERT_EXPR(large::count == 1);
        TC_ASSERT_EXPR(Tp::count == 1);
        assertContains<large>(lhs, 2);
        assertContains<Tp>(rhs, 1);
    }
#endif
}

int tc_utilities_any_any_class_any_assign_copy(void) {
    globalMemCounter.reset();
    test_copy_assign<small1, small2>();
    test_copy_assign<large1, large2>();
    test_copy_assign<small, large>();
    test_copy_assign<large, small>();
    test_copy_assign_empty<small>();
    test_copy_assign_empty<large>();
    test_copy_assign_self();
    test_copy_assign_throws<small_throws_on_copy>();
    test_copy_assign_throws<large_throws_on_copy>();

  return 0;
}
