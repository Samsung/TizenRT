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

// template <class T, class ...Args> T& emplace(Args&&...);
// template <class T, class U, class ...Args>
// T& emplace(initializer_list<U>, Args&&...);

#include <any>
#include <cassert>

#include "any_helpers.h"
#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Tracked {
    static int count;
    Tracked() { ++count; }
    Tracked(Tracked const&) noexcept { ++count; }
    Tracked& operator=(Tracked const&) = default;
    ~Tracked() { --count; }
};
int Tracked::count = 0;

template <class Type>
void test_emplace_type() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a(std::in_place_type<Tracked>);
        TC_ASSERT_EXPR(Tracked::count == 1);

        auto &v = a.emplace<Type>();
        static_assert( std::is_same_v<Type&, decltype(v)>, "" );
        TC_ASSERT_EXPR(&v == std::any_cast<Type>(&a));

        TC_ASSERT_EXPR(Tracked::count == 0);
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 0);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a(std::in_place_type<Tracked>);
        TC_ASSERT_EXPR(Tracked::count == 1);

        auto &v = a.emplace<Type>(101);
        static_assert( std::is_same_v<Type&, decltype(v)>, "" );
        TC_ASSERT_EXPR(&v == std::any_cast<Type>(&a));

        TC_ASSERT_EXPR(Tracked::count == 0);
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 101);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
    {
        std::any a(std::in_place_type<Tracked>);
        TC_ASSERT_EXPR(Tracked::count == 1);

        auto &v = a.emplace<Type>(-1, 42, -1);
        static_assert( std::is_same_v<Type&, decltype(v)>, "" );
        TC_ASSERT_EXPR(&v == std::any_cast<Type>(&a));

        TC_ASSERT_EXPR(Tracked::count == 0);
        TC_ASSERT_EXPR(Type::count == 1);
        TC_ASSERT_EXPR(Type::copied == 0);
        TC_ASSERT_EXPR(Type::moved == 0);
        assertContains<Type>(a, 42);
    }
    TC_ASSERT_EXPR(Type::count == 0);
    Type::reset();
}

template <class Type>
void test_emplace_type_tracked() {
    // constructing from a small type should perform no allocations.
    DisableAllocationGuard g(isSmallType<Type>()); ((void)g);
    {
        std::any a(std::in_place_type<Tracked>);
        TC_ASSERT_EXPR(Tracked::count == 1);
        auto &v = a.emplace<Type>();
        static_assert( std::is_same_v<Type&, decltype(v)>, "" );
        TC_ASSERT_EXPR(&v == std::any_cast<Type>(&a));

        TC_ASSERT_EXPR(Tracked::count == 0);
        assertArgsMatch<Type>(a);
    }
    {
        std::any a(std::in_place_type<Tracked>);
        TC_ASSERT_EXPR(Tracked::count == 1);
        auto &v = a.emplace<Type>(-1, 42, -1);
        static_assert( std::is_same_v<Type&, decltype(v)>, "" );
        TC_ASSERT_EXPR(&v == std::any_cast<Type>(&a));

        TC_ASSERT_EXPR(Tracked::count == 0);
        assertArgsMatch<Type, int, int, int>(a);
    }
    // initializer_list constructor tests
    {
        std::any a(std::in_place_type<Tracked>);
        TC_ASSERT_EXPR(Tracked::count == 1);
        auto &v = a.emplace<Type>({-1, 42, -1});
        static_assert( std::is_same_v<Type&, decltype(v)>, "" );
        TC_ASSERT_EXPR(&v == std::any_cast<Type>(&a));

        TC_ASSERT_EXPR(Tracked::count == 0);
        assertArgsMatch<Type, std::initializer_list<int>>(a);
    }
    {
        int x = 42;
        std::any a(std::in_place_type<Tracked>);
        TC_ASSERT_EXPR(Tracked::count == 1);
        auto &v = a.emplace<Type>({-1, 42, -1}, x);
        static_assert( std::is_same_v<Type&, decltype(v)>, "" );
        TC_ASSERT_EXPR(&v == std::any_cast<Type>(&a));

        TC_ASSERT_EXPR(Tracked::count == 0);
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
void test_emplace_throws()
{
    // any stores small type
    {
        std::any a(small{42});
        TC_ASSERT_EXPR(small::count == 1);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            auto &v = a.emplace<Type>(101);
            static_assert( std::is_same_v<Type&, decltype(v)>, "" );
            TC_ASSERT_EXPR(false);
        } catch (int const&) {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(small::count == 0);
    }
    {
        std::any a(small{42});
        TC_ASSERT_EXPR(small::count == 1);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            auto &v = a.emplace<Type>({1, 2, 3}, 101);
            static_assert( std::is_same_v<Type&, decltype(v)>, "" );
            TC_ASSERT_EXPR(false);
        } catch (int const&) {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(small::count == 0);
    }
    // any stores large type
    {
        std::any a(large{42});
        TC_ASSERT_EXPR(large::count == 1);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            auto &v = a.emplace<Type>(101);
            static_assert( std::is_same_v<Type&, decltype(v)>, "" );
            TC_ASSERT_EXPR(false);
        } catch (int const&) {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(large::count == 0);
    }
    {
        std::any a(large{42});
        TC_ASSERT_EXPR(large::count == 1);
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            auto &v = a.emplace<Type>({1, 2, 3}, 101);
            static_assert( std::is_same_v<Type&, decltype(v)>, "" );
            TC_ASSERT_EXPR(false);
        } catch (int const&) {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(large::count == 0);
    }
}

#endif

template <class T, class ...Args>
constexpr auto has_emplace(int)
    -> decltype(std::any{}.emplace<T>(std::declval<Args>()...), true) { return true; }

template <class ...Args>
constexpr bool has_emplace(long) { return false; }

template <class ...Args>
constexpr bool has_emplace() { return has_emplace<Args...>(0); }


template <class T, class IT, class ...Args>
constexpr auto has_emplace_init_list(int)
    -> decltype(std::any{}.emplace<T>(
        {std::declval<IT>(), std::declval<IT>(), std::declval<IT>()},
        std::declval<Args>()...), true) { return true; }

template <class ...Args>
constexpr bool has_emplace_init_list(long) { return false; }

template <class ...Args>
constexpr bool has_emplace_init_list() { return has_emplace_init_list<Args...>(0); }


void test_emplace_sfinae_constraints() {
    {
        static_assert(has_emplace<int>(), "");
        static_assert(has_emplace<int, int>(), "");
        static_assert(!has_emplace<int, int, int>(), "not constructible");
        static_assert(!has_emplace_init_list<int, int>(), "not constructible from il");
    }
    {
        static_assert(has_emplace<small>(), "");
        static_assert(has_emplace<large>(), "");
        static_assert(!has_emplace<small, void*>(), "");
        static_assert(!has_emplace<large, void*>(), "");

        static_assert(has_emplace_init_list<small, int>(), "");
        static_assert(has_emplace_init_list<large, int>(), "");
        static_assert(!has_emplace_init_list<small, void*>(), "");
        static_assert(!has_emplace_init_list<large, void*>(), "");
    }
    {
        // Test that the emplace SFINAE's away when the
        // argument is non-copyable
        struct NoCopy {
          NoCopy() = default;
          NoCopy(NoCopy const&) = delete;
          NoCopy(int) {}
          NoCopy(std::initializer_list<int>, int, int) {}
        };
        static_assert(!has_emplace<NoCopy>(), "");
        static_assert(!has_emplace<NoCopy, int>(), "");
        static_assert(!has_emplace_init_list<NoCopy, int, int, int>(), "");
        static_assert(!has_emplace<NoCopy&>(), "");
        static_assert(!has_emplace<NoCopy&, int>(), "");
        static_assert(!has_emplace_init_list<NoCopy&, int, int, int>(), "");
        static_assert(!has_emplace<NoCopy&&>(), "");
        static_assert(!has_emplace<NoCopy&&, int>(), "");
        static_assert(!has_emplace_init_list<NoCopy&&, int, int, int>(), "");

    }
}

int tc_utilities_any_any_class_any_modifiers_emplace(void) {
    test_emplace_type<small>();
    test_emplace_type<large>();
    test_emplace_type<small_throws_on_copy>();
    test_emplace_type<large_throws_on_copy>();
    test_emplace_type<throws_on_move>();
    test_emplace_type_tracked<small_tracked_t>();
    test_emplace_type_tracked<large_tracked_t>();
    test_emplace_sfinae_constraints();
#ifndef TEST_HAS_NO_EXCEPTIONS
    test_emplace_throws<SmallThrows>();
    test_emplace_throws<LargeThrows>();
#endif

  return 0;
}
