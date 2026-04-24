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

// template <class Value> any(Value &&)

// Test construction from a value.
// Concerns:
// ---------
// 1. The value is properly move/copied depending on the value category.
// 2. Both small and large values are properly handled.


#include <any>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class Type>
void test_copy_value_throws()
{
#if !defined(TEST_HAS_NO_EXCEPTIONS)
    TC_ASSERT_EXPR(Type::count == 0);
    {
        const Type t(42);
        TC_ASSERT_EXPR(Type::count == 1);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            std::any a2 = t;
            TC_ASSERT_EXPR(false);
        } catch (const my_any_exception&) {
            // do nothing
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(t.value == 42);
    }
    TC_ASSERT_EXPR(Type::count == 0);
#endif
}

void test_move_value_throws()
{
#if !defined(TEST_HAS_NO_EXCEPTIONS)
    TC_ASSERT_EXPR(throws_on_move::count == 0);
    {
        throws_on_move v;
        TC_ASSERT_EXPR(throws_on_move::count == 1);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            std::any a = std::move(v);
            TC_ASSERT_EXPR(false);
        } catch (const my_any_exception&) {
            // do nothing
        } catch (...) {
            TC_ASSERT_EXPR(false);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(throws_on_move::count == 1);
    }
    TC_ASSERT_EXPR(throws_on_move::count == 0);
#endif
}

template <class Type>
void test_copy_move_value() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        Type t(42);
        TC_ASSERT_EXPR(Type::count == 1);

        std::any a = t;

        TC_ASSERT_EXPR(Type::count == 2);
        TC_ASSERT_EXPR(Type::copied == 1);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 42);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        Type t(42);
        TC_ASSERT_EXPR(Type::count == 1);

        std::any a = std::move(t);

        TC_ASSERT_EXPR(Type::count == 2);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 1);
        assertContains<Type>(a, 42);
    }
}

// Test that any(ValueType&&) is *never* selected for a std::in_place_type_t specialization.
void test_sfinae_constraints() {
    using BadTag = std::in_place_type_t<int>;
    using OKTag = std::in_place_t;
    // Test that the tag type is properly handled in SFINAE
    BadTag t = std::in_place_type<int>;
    OKTag ot = std::in_place;
    {
        std::any a(t);
        assertContains<int>(a, 0);
    }
    {
        std::any a(std::move(t));
        assertContains<int>(a, 0);
    }
    {
        std::any a(ot);
        TC_ASSERT_EXPR(containsType<OKTag>(a));
    }
    {
        struct Dummy { Dummy() = delete; };
        using T = std::in_place_type_t<Dummy>;
        static_assert(!std::is_constructible<std::any, T>::value, "");
    }
    {
        // Test that the ValueType&& constructor SFINAE's away when the
        // argument is non-copyable
        struct NoCopy {
          NoCopy() = default;
          NoCopy(NoCopy const&) = delete;
          NoCopy(int) {}
        };
        static_assert(!std::is_constructible<std::any, NoCopy>::value, "");
        static_assert(!std::is_constructible<std::any, NoCopy&>::value, "");
        static_assert(!std::is_convertible<NoCopy, std::any>::value, "");
    }
}

int tc_utilities_any_any_class_any_cons_value(void) {
    test_copy_move_value<small>();
    test_copy_move_value<large>();
    test_copy_value_throws<small_throws_on_copy>();
    test_copy_value_throws<large_throws_on_copy>();
    test_move_value_throws();
    test_sfinae_constraints();

  return 0;
}
