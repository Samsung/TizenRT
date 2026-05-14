//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-bad_optional_access-missing && !no-exceptions

// <optional>

// constexpr optional(T&& v);

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"


using std::optional;


class Z
{
public:
    Z(int) {}
    Z(Z&&) {TEST_THROW(6);}
};


int tc_utilities_optional_optional_object_optional_object_ctor_rvalue_T(void) {
    {
        typedef int T;
        constexpr optional<T> opt(T(5));
        static_assert(static_cast<bool>(opt) == true, "");
        static_assert(*opt == 5, "");

        struct test_constexpr_ctor
            : public optional<T>
        {
            constexpr test_constexpr_ctor(T&&) {}
        };
    }
    {
        typedef double T;
        constexpr optional<T> opt(T(3));
        static_assert(static_cast<bool>(opt) == true, "");
        static_assert(*opt == 3, "");

        struct test_constexpr_ctor
            : public optional<T>
        {
            constexpr test_constexpr_ctor(T&&) {}
        };
    }
    {
        const int x = 42;
        optional<const int> o(std::move(x));
        TC_ASSERT_EXPR(*o == 42);
    }
    {
        typedef TestTypes::TestType T;
        T::reset();
        optional<T> opt = T{3};
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::move_constructed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(opt.value().value == 3);
    }
    {
        typedef ExplicitTestTypes::TestType T;
        static_assert(!std::is_convertible<T&&, optional<T>>::value, "");
        T::reset();
        optional<T> opt(T{3});
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::move_constructed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(opt.value().value == 3);
    }
    {
        typedef TestTypes::TestType T;
        T::reset();
        optional<T> opt = {3};
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::value_constructed == 1);
        TC_ASSERT_EXPR(T::copy_constructed == 0);
        TC_ASSERT_EXPR(T::move_constructed == 0);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(opt.value().value == 3);
    }
    {
        typedef ConstexprTestTypes::TestType T;
        constexpr optional<T> opt = {T(3)};
        static_assert(static_cast<bool>(opt) == true, "");
        static_assert(opt.value().value == 3, "");

        struct test_constexpr_ctor
            : public optional<T>
        {
            constexpr test_constexpr_ctor(const T&) {}
        };
    }
    {
        typedef ConstexprTestTypes::TestType T;
        constexpr optional<T> opt = {3};
        static_assert(static_cast<bool>(opt) == true, "");
        static_assert(opt.value().value == 3, "");

        struct test_constexpr_ctor
            : public optional<T>
        {
            constexpr test_constexpr_ctor(const T&) {}
        };
    }
    {
        typedef ExplicitConstexprTestTypes::TestType T;
        static_assert(!std::is_convertible<T&&, optional<T>>::value, "");
        constexpr optional<T> opt(T{3});
        static_assert(static_cast<bool>(opt) == true, "");
        static_assert(opt.value().value == 3, "");

        struct test_constexpr_ctor
            : public optional<T>
        {
            constexpr test_constexpr_ctor(T&&) {}
        };

    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        try
        {
            Z z(3);
            optional<Z> opt(std::move(z));
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 6);
        }
    }
#endif

  return 0;
}
