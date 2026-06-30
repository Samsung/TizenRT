//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// constexpr optional<T>& operator=(const optional<T>& rhs);

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"

using std::optional;

struct X
{
    static bool throw_now;

    X() = default;
    X(const X&)
    {
        if (throw_now)
            TEST_THROW(6);
    }
    X& operator=(X const&) = default;
};

bool X::throw_now = false;

template <class Tp>
constexpr bool assign_empty(optional<Tp>&& lhs) {
    const optional<Tp> rhs;
    lhs = rhs;
    return !lhs.has_value() && !rhs.has_value();
}

template <class Tp>
constexpr bool assign_value(optional<Tp>&& lhs) {
    const optional<Tp> rhs(101);
    lhs = rhs;
    return lhs.has_value() && rhs.has_value() && *lhs == *rhs;
}

int tc_utilities_optional_optional_object_optional_object_assign_copy(void) {
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
    {
        using O = optional<TestTypes::TestType>;
        TC_ASSERT_EXPR(assign_empty(O{42}));
        TC_ASSERT_EXPR(assign_value(O{42}));
    }
    {
        using T = TestTypes::TestType;
        T::reset();
        optional<T> opt(3);
        const optional<T> opt2;
        TC_ASSERT_EXPR(T::alive == 1);
        opt = opt2;
        TC_ASSERT_EXPR(T::alive == 0);
        TC_ASSERT_EXPR(!opt2.has_value());
        TC_ASSERT_EXPR(!opt.has_value());
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        optional<X> opt;
        optional<X> opt2(X{});
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        try
        {
            X::throw_now = true;
            opt = opt2;
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 6);
            TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
        }
    }
#endif

    TC_SUCCESS_RESULT();
    return 0;
}
