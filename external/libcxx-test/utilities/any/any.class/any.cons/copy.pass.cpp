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

// any(any const &);

#include <any>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class Type>
void test_copy_throws() {
#if !defined(TEST_HAS_NO_EXCEPTIONS)
    TC_ASSERT_EXPR(Type::count == 0);
    {
        const std::any a = Type(42);
        TC_ASSERT_EXPR(Type::count == 1);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            const std::any a2(a);
            TC_ASSERT_EXPR(false);
        } catch (my_any_exception const &) {
            // do nothing
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(Type::count == 1);
        assertContains<Type>(a, 42);
    }
    TC_ASSERT_EXPR(Type::count == 0);
#endif
}

void test_copy_empty() {
    DisableAllocationGuard g; ((void)g); // No allocations should occur.
    std::any a1;
    std::any a2(a1);

    assertEmpty(a1);
    assertEmpty(a2);
}

template <class Type>
void test_copy()
{
    // Copying small types should not perform any allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a = Type(42);
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);

        std::any a2(a);

        TC_ASSERT_EXPR(Type::copied == 1);
        TC_ASSERT_EXPR(Type::count == 2);
        assertContains<Type>(a, 42);
        assertContains<Type>(a2, 42);

        // Modify a and check that a2 is unchanged
        modifyValue<Type>(a, -1);
        assertContains<Type>(a, -1);
        assertContains<Type>(a2, 42);

        // modify a2 and check that a is unchanged
        modifyValue<Type>(a2, 999);
        assertContains<Type>(a, -1);
        assertContains<Type>(a2, 999);

        // clear a and check that a2 is unchanged
        a.reset();
        assertEmpty(a);
        assertContains<Type>(a2, 999);
    }
    TC_ASSERT_EXPR(Type::count == 0);
}

int tc_utilities_any_any_class_any_cons_copy(void) {
    test_copy<small>();
    test_copy<large>();
    test_copy_empty();
    test_copy_throws<small_throws_on_copy>();
    test_copy_throws<large_throws_on_copy>();

  return 0;
}
