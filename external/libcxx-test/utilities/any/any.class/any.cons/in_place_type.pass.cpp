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

// template <class T, class ...Args> any(in_place_type_t<T>, Args&&...);
// template <class T, class U, class ...Args>
// any(in_place_type_t<T>, initializer_list<U>, Args&&...);

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
#include "test_convertible.h"
#include "libcxx_tc_common.h"

template <class Type>
void test_in_place_type() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a(std::in_place_type<Type>);

        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 0);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    { // Test that the in_place argument is properly decayed
        std::any a(std::in_place_type<Type&>);

        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 0);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a(std::in_place_type<Type>, 101);

        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 101);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a(std::in_place_type<Type>, -1, 42, -1);

        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 42);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
}

template <class Type>
void test_in_place_type_tracked() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    {
        std::any a(std::in_place_type<Type>);
        assertArgsMatch<Type>(a);
    }
    {
        std::any a(std::in_place_type<Type>, -1, 42, -1);
        assertArgsMatch<Type, int, int, int>(a);
    }
    // initializer_list constructor tests
    {
        std::any a(std::in_place_type<Type>, {-1, 42, -1});
        assertArgsMatch<Type, std::initializer_list<int>>(a);
    }
    {
        int x = 42;
        std::any a(std::in_place_type<Type&>, {-1, 42, -1}, x);
        assertArgsMatch<Type, std::initializer_list<int>, int&>(a);
    }
}

void test_func() {}

void test_in_place_type_decayed() {
    {
        using Type = decltype(test_func);
        using DecayT = void(*)();
        std::any a(std::in_place_type<Type>, test_func);
        TC_ASSERT_EXPR(containsType<DecayT>(a));
        TC_ASSERT_EXPR(std::any_cast<DecayT>(a) == test_func);
    }
    {
        int my_arr[5];
        using Type = int(&)[5];
        using DecayT = int*;
        std::any a(std::in_place_type<Type>, my_arr);
        TC_ASSERT_EXPR(containsType<DecayT>(a));
        TC_ASSERT_EXPR(std::any_cast<DecayT>(a) == my_arr);
    }
    {
        using Type = int[5];
        using DecayT = int*;
        std::any a(std::in_place_type<Type>);
        TC_ASSERT_EXPR(containsType<DecayT>(a));
        TC_ASSERT_EXPR(std::any_cast<DecayT>(a) == nullptr);
    }
}

void test_ctor_sfinae() {
    {
        // Test that the init-list ctor SFINAE's away properly when
        // construction would be ill-formed.
        using IL = std::initializer_list<int>;
        static_assert(!std::is_constructible<std::any,
                      std::in_place_type_t<int>, IL>::value, "");
        static_assert(std::is_constructible<std::any,
            std::in_place_type_t<small_tracked_t>, IL>::value, "");
    }
    {
        // Test that the tagged dispatch constructor SFINAE's away when the
        // argument is non-copyable
        struct NoCopy {
          NoCopy() = default;
          NoCopy(NoCopy const&) = delete;
          NoCopy(int) {}
          NoCopy(std::initializer_list<int>, int) {}
        };
        using Tag = std::in_place_type_t<NoCopy>;
        using RefTag = std::in_place_type_t<NoCopy&>;
        using IL = std::initializer_list<int>;
        static_assert(!std::is_constructible<std::any, Tag>::value, "");
        static_assert(!std::is_constructible<std::any, Tag, int>::value, "");
        static_assert(!std::is_constructible<std::any, Tag, IL, int>::value, "");
        static_assert(!std::is_constructible<std::any, RefTag>::value, "");
        static_assert(!std::is_constructible<std::any, RefTag, int>::value, "");
        static_assert(!std::is_constructible<std::any, RefTag, IL, int>::value, "");
    }
}

struct Implicit {
  Implicit(int) {}
  Implicit(int, int, int) {}
  Implicit(std::initializer_list<int>, int) {}
};

void test_constructor_explicit() {
    using I = Implicit;
    using IT = std::in_place_type_t<I>;
    static_assert(!test_convertible<std::any, IT, int>(), "");
    static_assert(std::is_constructible<std::any, IT, int>::value, "");
    static_assert(!test_convertible<std::any, IT, int, int, int>(), "");
    static_assert(std::is_constructible<std::any, IT, int, int, int>::value, "");
    static_assert(!test_convertible<std::any, IT, std::initializer_list<int>&, int>(), "");
    static_assert(std::is_constructible<std::any, IT, std::initializer_list<int>&, int>::value, "");
}

int tc_utilities_any_any_class_any_cons_in_place_type(void) {
    test_in_place_type<small>();
    test_in_place_type<large>();
    test_in_place_type<small_throws_on_copy>();
    test_in_place_type<large_throws_on_copy>();
    test_in_place_type<throws_on_move>();
    test_in_place_type_tracked<small_tracked_t>();
    test_in_place_type_tracked<large_tracked_t>();
    test_in_place_type_decayed();
    test_ctor_sfinae();
    test_constructor_explicit();

  return 0;
}
