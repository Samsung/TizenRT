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
//
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-bad_optional_access-missing && !no-exceptions

// <optional>

// template <class U>
//   constexpr EXPLICIT optional(U&& u);

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"
#include "test_convertible.h"
#include "libcxx_tc_common.h"


using std::optional;

struct ImplicitThrow
{
    constexpr ImplicitThrow(int x) { if (x != -1) TEST_THROW(6);}
};

struct ExplicitThrow
{
    constexpr explicit ExplicitThrow(int x) { if (x != -1) TEST_THROW(6);}
};

struct ImplicitAny {
  template <class U>
  constexpr ImplicitAny(U&&) {}
};


template <class To, class From>
constexpr bool implicit_conversion(optional<To>&& opt, const From& v)
{
    using O = optional<To>;
    static_assert(test_convertible<O, From>(), "");
    static_assert(!test_convertible<O, void*>(), "");
    static_assert(!test_convertible<O, From, int>(), "");
    return opt && *opt == static_cast<To>(v);
}

template <class To, class Input, class Expect>
constexpr bool explicit_conversion(Input&& in, const Expect& v)
{
    using O = optional<To>;
    static_assert(std::is_constructible<O, Input>::value, "");
    static_assert(!std::is_convertible<Input, O>::value, "");
    static_assert(!std::is_constructible<O, void*>::value, "");
    static_assert(!std::is_constructible<O, Input, int>::value, "");
    optional<To> opt(std::forward<Input>(in));
    return opt && *opt == static_cast<To>(v);
}

void test_implicit()
{
    {
        static_assert(implicit_conversion<long long>(42, 42), "");
    }
    {
        static_assert(implicit_conversion<long double>(3.14, 3.14), "");
    }
    {
        int x = 42;
        optional<void* const> o(&x);
        TC_ASSERT_EXPR(*o == &x);
    }
    {
        using T = TrivialTestTypes::TestType;
        static_assert(implicit_conversion<T>(42, 42), "");
    }
    {
        using T = TestTypes::TestType;
        TC_ASSERT_EXPR(implicit_conversion<T>(3, T(3)));
    }
  {
    using O = optional<ImplicitAny>;
    static_assert(!test_convertible<O, std::in_place_t>(), "");
    static_assert(!test_convertible<O, std::in_place_t&>(), "");
    static_assert(!test_convertible<O, const std::in_place_t&>(), "");
    static_assert(!test_convertible<O, std::in_place_t&&>(), "");
    static_assert(!test_convertible<O, const std::in_place_t&&>(), "");

  }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            using T = ImplicitThrow;
            optional<T> t = 42;
            TC_ASSERT_EXPR(false);
            ((void)t);
        } catch (int) {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
#endif
}

void test_explicit() {
    {
        using T = ExplicitTrivialTestTypes::TestType;
        static_assert(explicit_conversion<T>(42, 42), "");
    }
    {
        using T = ExplicitConstexprTestTypes::TestType;
        static_assert(explicit_conversion<T>(42, 42), "");
        static_assert(!std::is_convertible<int, T>::value, "");
    }
    {
        using T = ExplicitTestTypes::TestType;
        T::reset();
        {
            TC_ASSERT_EXPR(explicit_conversion<T>(42, 42));
            TC_ASSERT_EXPR(T::alive == 0);
        }
        T::reset();
        {
            optional<T> t(42);
            TC_ASSERT_EXPR(T::alive == 1);
            TC_ASSERT_EXPR(T::value_constructed == 1);
            TC_ASSERT_EXPR(T::move_constructed == 0);
            TC_ASSERT_EXPR(T::copy_constructed == 0);
            TC_ASSERT_EXPR(t.value().value == 42);
        }
        TC_ASSERT_EXPR(T::alive == 0);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            using T = ExplicitThrow;
            optional<T> t(42);
            TC_ASSERT_EXPR(false);
        } catch (int) {
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
#endif
}

int tc_utilities_optional_optional_object_optional_object_ctor_U(void) {
    test_implicit();
    test_explicit();

  return 0;
}
