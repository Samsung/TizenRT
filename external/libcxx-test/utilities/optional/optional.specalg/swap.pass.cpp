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

// template <class T> void swap(optional<T>& x, optional<T>& y)
//     noexcept(noexcept(x.swap(y)));

#include <optional>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"

using std::optional;

class X
{
    int i_;
public:
    static unsigned dtor_called;
    X(int i) : i_(i) {}
    X(X&& x) = default;
    X& operator=(X&&) = default;
    ~X() {++dtor_called;}

    friend bool operator==(const X& x, const X& y) {return x.i_ == y.i_;}
};

unsigned X::dtor_called = 0;

class Y
{
    int i_;
public:
    static unsigned dtor_called;
    Y(int i) : i_(i) {}
    Y(Y&&) = default;
    ~Y() {++dtor_called;}

    friend constexpr bool operator==(const Y& x, const Y& y) {return x.i_ == y.i_;}
    friend void swap(Y& x, Y& y) {std::swap(x.i_, y.i_);}
};

unsigned Y::dtor_called = 0;

class Z
{
    int i_;
public:
    Z(int i) : i_(i) {}
    Z(Z&&) { TEST_THROW(7);}

    friend constexpr bool operator==(const Z& x, const Z& y) {return x.i_ == y.i_;}
    friend void swap(Z&, Z&) { TEST_THROW(6);}
};


struct NonSwappable {
    NonSwappable(NonSwappable const&) = delete;
};
void swap(NonSwappable&, NonSwappable&) = delete;

void test_swap_sfinae() {
    using std::optional;
    {
        using T = TestTypes::TestType;
        static_assert(std::is_swappable_v<optional<T>>, "");
    }
    {
        using T = TestTypes::MoveOnly;
        static_assert(std::is_swappable_v<optional<T>>, "");
    }
    {
        using T = TestTypes::Copyable;
        static_assert(std::is_swappable_v<optional<T>>, "");
    }
    {
        using T = TestTypes::NoCtors;
        static_assert(!std::is_swappable_v<optional<T>>, "");
    }
    {
        using T = NonSwappable;
        static_assert(!std::is_swappable_v<optional<T>>, "");
    }
    {
        // Even though CopyOnly has deleted move operations, those operations
        // cause optional<CopyOnly> to have implicitly deleted move operations
        // that decay into copies.
        using T = TestTypes::CopyOnly;
        using Opt = optional<T>;
        T::reset();
        Opt L(101), R(42);
        T::reset_constructors();
        std::swap(L, R);
        TC_ASSERT_EXPR(L->value == 42);
        TC_ASSERT_EXPR(R->value == 101);
        TC_ASSERT_EXPR(T::copy_constructed == 1);
        TC_ASSERT_EXPR(T::constructed == T::copy_constructed);
        TC_ASSERT_EXPR(T::assigned == 2);
        TC_ASSERT_EXPR(T::assigned == T::copy_assigned);
    }
}

int tc_utilities_optional_optional_specalg_swap(void) {
    test_swap_sfinae();
    {
        optional<int> opt1;
        optional<int> opt2;
        static_assert(noexcept(swap(opt1, opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        swap(opt1, opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<int> opt1(1);
        optional<int> opt2;
        static_assert(noexcept(swap(opt1, opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        swap(opt1, opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<int> opt1;
        optional<int> opt2(2);
        static_assert(noexcept(swap(opt1, opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        swap(opt1, opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<int> opt1(1);
        optional<int> opt2(2);
        static_assert(noexcept(swap(opt1, opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        swap(opt1, opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<X> opt1;
        optional<X> opt2;
        static_assert(noexcept(swap(opt1, opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        swap(opt1, opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        TC_ASSERT_EXPR(X::dtor_called == 0);
    }
    {
        optional<X> opt1(1);
        optional<X> opt2;
        static_assert(noexcept(swap(opt1, opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        X::dtor_called = 0;
        swap(opt1, opt2);
        TC_ASSERT_EXPR(X::dtor_called == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<X> opt1;
        optional<X> opt2(2);
        static_assert(noexcept(swap(opt1, opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        X::dtor_called = 0;
        swap(opt1, opt2);
        TC_ASSERT_EXPR(X::dtor_called == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<X> opt1(1);
        optional<X> opt2(2);
        static_assert(noexcept(swap(opt1, opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        X::dtor_called = 0;
        swap(opt1, opt2);
        TC_ASSERT_EXPR(X::dtor_called == 1);  // from inside std::swap
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<Y> opt1;
        optional<Y> opt2;
        static_assert(noexcept(swap(opt1, opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        swap(opt1, opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        TC_ASSERT_EXPR(Y::dtor_called == 0);
    }
    {
        optional<Y> opt1(1);
        optional<Y> opt2;
        static_assert(noexcept(swap(opt1, opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        Y::dtor_called = 0;
        swap(opt1, opt2);
        TC_ASSERT_EXPR(Y::dtor_called == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<Y> opt1;
        optional<Y> opt2(2);
        static_assert(noexcept(swap(opt1, opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        Y::dtor_called = 0;
        swap(opt1, opt2);
        TC_ASSERT_EXPR(Y::dtor_called == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<Y> opt1(1);
        optional<Y> opt2(2);
        static_assert(noexcept(swap(opt1, opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        Y::dtor_called = 0;
        swap(opt1, opt2);
        TC_ASSERT_EXPR(Y::dtor_called == 0);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<Z> opt1;
        optional<Z> opt2;
        static_assert(noexcept(swap(opt1, opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        swap(opt1, opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        optional<Z> opt1;
        opt1.emplace(1);
        optional<Z> opt2;
        static_assert(noexcept(swap(opt1, opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        try
        {
            swap(opt1, opt2);
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 7);
        }
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<Z> opt1;
        optional<Z> opt2;
        opt2.emplace(2);
        static_assert(noexcept(swap(opt1, opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        try
        {
            swap(opt1, opt2);
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 7);
        }
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
    }
    {
        optional<Z> opt1;
        opt1.emplace(1);
        optional<Z> opt2;
        opt2.emplace(2);
        static_assert(noexcept(swap(opt1, opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        try
        {
            swap(opt1, opt2);
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 6);
        }
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
    }
#endif // TEST_HAS_NO_EXCEPTIONS

  return 0;
}
