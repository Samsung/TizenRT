//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// From LWG2451:
// template<class U>
//   optional<T>& operator=(const optional<U>& rhs);

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"

using std::optional;

namespace {

struct X
{
    static bool throw_now;

    X() = default;
    X(int)
    {
        if (throw_now)
            TEST_THROW(6);
    }
};

bool X::throw_now = false;

struct Y1
{
    Y1() = default;
    Y1(const int&) {}
    Y1& operator=(const Y1&) = delete;
};

struct Y2
{
    Y2() = default;
    Y2(const int&) = delete;
    Y2& operator=(const int&) { return *this; }
};

template <class T>
struct AssignableFrom {
  static int type_constructed;
  static int type_assigned;
static int int_constructed;
  static int int_assigned;

  static void reset() {
      type_constructed = int_constructed = 0;
      type_assigned = int_assigned = 0;
  }

  AssignableFrom() = default;

  explicit AssignableFrom(T) { ++type_constructed; }
  AssignableFrom& operator=(T) { ++type_assigned; return *this; }

  AssignableFrom(int) { ++int_constructed; }
  AssignableFrom& operator=(int) { ++int_assigned; return *this; }
private:
  AssignableFrom(AssignableFrom const&) = delete;
  AssignableFrom& operator=(AssignableFrom const&) = delete;
};

template <class T> int AssignableFrom<T>::type_constructed = 0;
template <class T> int AssignableFrom<T>::type_assigned = 0;
template <class T> int AssignableFrom<T>::int_constructed = 0;
template <class T> int AssignableFrom<T>::int_assigned = 0;


void test_with_test_type() {
    using T = TestTypes::TestType;
    T::reset();
    { // non-empty to empty
        T::reset_constructors();
        optional<T> opt;
        const optional<int> other(42);
        opt = other;
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 1);
        TC_ASSERT_EXPR(T::value_constructed == 1);
        TC_ASSERT_EXPR(T::assigned == 0);
        TC_ASSERT_EXPR(T::destroyed == 0);
        TC_ASSERT_EXPR(static_cast<bool>(other) == true);
        TC_ASSERT_EXPR(*other == 42);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(42));
    }
    TC_ASSERT_EXPR(T::alive == 0);
    { // non-empty to non-empty
        optional<T> opt(101);
        const optional<int> other(42);
        T::reset_constructors();
        opt = other;
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 0);
        TC_ASSERT_EXPR(T::assigned == 1);
        TC_ASSERT_EXPR(T::value_assigned == 1);
        TC_ASSERT_EXPR(T::destroyed == 0);
        TC_ASSERT_EXPR(static_cast<bool>(other) == true);
        TC_ASSERT_EXPR(*other == 42);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(42));
    }
    TC_ASSERT_EXPR(T::alive == 0);
    { // empty to non-empty
        optional<T> opt(101);
        const optional<int> other;
        T::reset_constructors();
        opt = other;
        TC_ASSERT_EXPR(T::alive == 0);
        TC_ASSERT_EXPR(T::constructed == 0);
        TC_ASSERT_EXPR(T::assigned == 0);
        TC_ASSERT_EXPR(T::destroyed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(other) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    TC_ASSERT_EXPR(T::alive == 0);
    { // empty to empty
        optional<T> opt;
        const optional<int> other;
        T::reset_constructors();
        opt = other;
        TC_ASSERT_EXPR(T::alive == 0);
        TC_ASSERT_EXPR(T::constructed == 0);
        TC_ASSERT_EXPR(T::assigned == 0);
        TC_ASSERT_EXPR(T::destroyed == 0);
        TC_ASSERT_EXPR(static_cast<bool>(other) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
    }
    TC_ASSERT_EXPR(T::alive == 0);
}

void test_ambiguous_assign() {
    using OptInt = std::optional<int>;
    {
        using T = AssignableFrom<OptInt const&>;
        const OptInt a(42);
        T::reset();
        {
            std::optional<T> t;
            t = a;
            TC_ASSERT_EXPR(T::type_constructed == 1);
            TC_ASSERT_EXPR(T::type_assigned == 0);
            TC_ASSERT_EXPR(T::int_constructed == 0);
            TC_ASSERT_EXPR(T::int_assigned == 0);
        }
        T::reset();
        {
            std::optional<T> t(42);
            t = a;
            TC_ASSERT_EXPR(T::type_constructed == 0);
            TC_ASSERT_EXPR(T::type_assigned == 1);
            TC_ASSERT_EXPR(T::int_constructed == 1);
            TC_ASSERT_EXPR(T::int_assigned == 0);
        }
        T::reset();
        {
            std::optional<T> t(42);
            t = std::move(a);
            TC_ASSERT_EXPR(T::type_constructed == 0);
            TC_ASSERT_EXPR(T::type_assigned == 1);
            TC_ASSERT_EXPR(T::int_constructed == 1);
            TC_ASSERT_EXPR(T::int_assigned == 0);
        }
    }
    {
        using T = AssignableFrom<OptInt&>;
        OptInt a(42);
        T::reset();
        {
            std::optional<T> t;
            t = a;
            TC_ASSERT_EXPR(T::type_constructed == 1);
            TC_ASSERT_EXPR(T::type_assigned == 0);
            TC_ASSERT_EXPR(T::int_constructed == 0);
            TC_ASSERT_EXPR(T::int_assigned == 0);
        }
        {
            using Opt = std::optional<T>;
            static_assert(!std::is_assignable_v<Opt&, OptInt const&>, "");
        }
    }
}

} // namespace

int tc_utilities_optional_optional_object_optional_object_assign_const_optional_U(void) {
    test_with_test_type();
    test_ambiguous_assign();
    {
        optional<int> opt;
        constexpr optional<short> opt2;
        opt = opt2;
        static_assert(static_cast<bool>(opt2) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
    }
    {
        optional<int> opt;
        constexpr optional<short> opt2(short{2});
        opt = opt2;
        static_assert(static_cast<bool>(opt2) == true, "");
        static_assert(*opt2 == 2, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
        TC_ASSERT_EXPR(*opt == *opt2);
    }
    {
        optional<int> opt(3);
        constexpr optional<short> opt2;
        opt = opt2;
        static_assert(static_cast<bool>(opt2) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
    }
    {
        optional<int> opt(3);
        constexpr optional<short> opt2(short{2});
        opt = opt2;
        static_assert(static_cast<bool>(opt2) == true, "");
        static_assert(*opt2 == 2, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt) == static_cast<bool>(opt2));
        TC_ASSERT_EXPR(*opt == *opt2);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        optional<X> opt;
        optional<int> opt2(42);
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
