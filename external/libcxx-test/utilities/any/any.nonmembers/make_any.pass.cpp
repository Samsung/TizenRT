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

// template <class T, class ...Args> any make_any(Args&&...);
// template <class T, class U, class ...Args>
// any make_any(initializer_list<U>, Args&&...);

#include <any>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class Type>
void test_make_any_type() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a = std::make_any<Type>();

        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 0);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a = std::make_any<Type>(101);

        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 101);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a = std::make_any<Type>(-1, 42, -1);

        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 42);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
}

template <class Type>
void test_make_any_type_tracked() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    {
        std::any a = std::make_any<Type>();
        assertArgsMatch<Type>(a);
    }
    {
        std::any a = std::make_any<Type>(-1, 42, -1);
        assertArgsMatch<Type, int, int, int>(a);
    }
    // initializer_list constructor tests
    {
        std::any a = std::make_any<Type>({-1, 42, -1});
        assertArgsMatch<Type, std::initializer_list<int>>(a);
    }
    {
        int x = 42;
        std::any a  = std::make_any<Type>({-1, 42, -1}, x);
        assertArgsMatch<Type, std::initializer_list<int>, int&>(a);
    }
}

#ifndef TEST_HAS_NO_EXCEPTIONS

struct SmallThrows {
  SmallThrows(int) { throw 42; }
  SmallThrows(std::initializer_list<int>, int) { throw 42; }
};
static_assert(IsSmallObject<SmallThrows>::value, "");

struct LargeThrows {
  LargeThrows(int) { throw 42; }
  LargeThrows(std::initializer_list<int>, int) { throw 42; }
  int data[sizeof(std::any)];
};
static_assert(!IsSmallObject<LargeThrows>::value, "");

template <class Type>
void test_make_any_throws()
{
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            TEST_IGNORE_NODISCARD std::make_any<Type>(101);
            TC_ASSERT_EXPR(false);
        } catch (int const&) {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            TEST_IGNORE_NODISCARD std::make_any<Type>({1, 2, 3}, 101);
            TC_ASSERT_EXPR(false);
        } catch (int const&) {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
}

#endif

int tc_utilities_any_any_nonmembers_make_any(void) {
    test_make_any_type<small>();
    test_make_any_type<large>();
    test_make_any_type<small_throws_on_copy>();
    test_make_any_type<large_throws_on_copy>();
    test_make_any_type<throws_on_move>();
    test_make_any_type_tracked<small_tracked_t>();
    test_make_any_type_tracked<large_tracked_t>();
#ifndef TEST_HAS_NO_EXCEPTIONS
    test_make_any_throws<SmallThrows>();
    test_make_any_throws<LargeThrows>();

#endif

  return 0;
}
