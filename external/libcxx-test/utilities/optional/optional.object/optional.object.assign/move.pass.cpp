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
// <optional>

// constexpr optional<T>& operator=(optional<T>&& rhs)
//     noexcept(is_nothrow_move_assignable<T>::value &&
//              is_nothrow_move_constructible<T>::value);

#include <optional>
#include <cassert>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"

using std::optional;

struct X
{
    static bool throw_now;
    static int alive;

    X() { ++alive; }
    X(X&&)
    {
        if (throw_now)
            TEST_THROW(6);
        ++alive;
    }

    X& operator=(X&&)
    {
        if (throw_now)
            TEST_THROW(42);
        return *this;
    }

    ~X() { TC_ASSERT_EXPR(alive > 0); --alive; }
};

struct Y {};

bool X::throw_now = false;
int X::alive = 0;


template <class Tp>
constexpr bool assign_empty(optional<Tp>&& lhs) {
    optional<Tp> rhs;
    lhs = std::move(rhs);
    return !lhs.has_value() && !rhs.has_value();
}

template <class Tp>
constexpr bool assign_value(optional<Tp>&& lhs) {
    optional<Tp> rhs(101);
    lhs = std::move(rhs);
    return lhs.has_value() && rhs.has_value() && *lhs == Tp{101};
}

int tc_utilities_optional_optional_object_optional_object_assign_move(void) {
    {
        static_assert(std::is_nothrow_move_assignable<optional<int>>::value, "");
        optional<int> opt;
        constexpr optional<int> opt2;
        opt = std::move(opt2);
        static_assert(static_cast<bool>(opt2) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
    }
    {
        optional<int> opt;
        constexpr optional<int> opt2(2);
        opt = std::move(opt2);
        static_assert(static_cast<bool>(opt2) == true, "");
        static_assert(*opt2 == 2, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
        TC_ASSERT_EXPR(*opt == *opt2);
    }
    {
        optional<int> opt(3);
        constexpr optional<int> opt2;
        opt = std::move(opt2);
        static_assert(static_cast<bool>(opt2) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
    }
    {
        using T = TestTypes::TestType;
        T::reset();
        optional<T> opt(3);
        optional<T> opt2;
        TC_ASSERT_EXPR(T::alive == 1);
        opt = std::move(opt2);
        TC_ASSERT_EXPR(T::alive == 0);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
    }
    {
        optional<int> opt(3);
        constexpr optional<int> opt2(2);
        opt = std::move(opt2);
        static_assert(static_cast<bool>(opt2) == true, "");
        static_assert(*opt2 == 2, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
        TC_ASSERT_EXPR(*opt == *opt2);
    }
    {
        using O = optional<int>;
        static_assert(assign_empty(O{42}));
        static_assert(assign_value(O{42}));
        TC_ASSERT_EXPR(assign_empty(O{42}));
        TC_ASSERT_EXPR(assign_value(O{42}));
    }
    {
        using O = optional<TrivialTestTypes::TestType>;
        static_assert(assign_empty(O{42}));
        static_assert(assign_value(O{42}));
        TC_ASSERT_EXPR(assign_empty(O{42}));
        TC_ASSERT_EXPR(assign_value(O{42}));
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        static_assert(!std::is_nothrow_move_assignable<optional<X>>::value, "");
        X::alive = 0;
        X::throw_now = false;
        optional<X> opt;
        optional<X> opt2(X{});
        TC_ASSERT_EXPR(X::alive == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        try
        {
            X::throw_now = true;
            opt = std::move(opt2);
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 6);
            TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
        }
        TC_ASSERT_EXPR(X::alive == 1);
    }
    TC_ASSERT_EXPR(X::alive == 0);
    {
        static_assert(!std::is_nothrow_move_assignable<optional<X>>::value, "");
        X::throw_now = false;
        optional<X> opt(X{});
        optional<X> opt2(X{});
        TC_ASSERT_EXPR(X::alive == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        try
        {
            X::throw_now = true;
            opt = std::move(opt2);
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 42);
            TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        }
        TC_ASSERT_EXPR(X::alive == 2);
    }
    TC_ASSERT_EXPR(X::alive == 0);
#endif // TEST_HAS_NO_EXCEPTIONS
    {
        static_assert(std::is_nothrow_move_assignable<optional<Y>>::value, "");
    }
    {
        struct ThrowsMove {
            ThrowsMove() noexcept {}
            ThrowsMove(ThrowsMove const&) noexcept {}
            ThrowsMove(ThrowsMove &&) noexcept(false) {}
            ThrowsMove& operator=(ThrowsMove const&) noexcept { return *this; }
            ThrowsMove& operator=(ThrowsMove &&) noexcept { return *this; }
        };
        static_assert(!std::is_nothrow_move_assignable<optional<ThrowsMove>>::value, "");
        struct ThrowsMoveAssign {
            ThrowsMoveAssign() noexcept {}
            ThrowsMoveAssign(ThrowsMoveAssign const&) noexcept {}
            ThrowsMoveAssign(ThrowsMoveAssign &&) noexcept {}
            ThrowsMoveAssign& operator=(ThrowsMoveAssign const&) noexcept { return *this; }
            ThrowsMoveAssign& operator=(ThrowsMoveAssign &&) noexcept(false) { return *this; }
        };
        static_assert(!std::is_nothrow_move_assignable<optional<ThrowsMoveAssign>>::value, "");
        struct NoThrowMove {
            NoThrowMove() noexcept(false) {}
            NoThrowMove(NoThrowMove const&) noexcept(false) {}
            NoThrowMove(NoThrowMove &&) noexcept {}
            NoThrowMove& operator=(NoThrowMove const&) noexcept { return *this; }
            NoThrowMove& operator=(NoThrowMove&&) noexcept { return *this; }
        };
        static_assert(std::is_nothrow_move_assignable<optional<NoThrowMove>>::value, "");
    }
    return 0;
}
