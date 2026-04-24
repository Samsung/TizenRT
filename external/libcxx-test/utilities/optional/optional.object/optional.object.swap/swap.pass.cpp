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

// void swap(optional&)
//     noexcept(is_nothrow_move_constructible<T>::value &&
//              is_nothrow_swappable<T>::value)

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
    Z(Z&&) {TEST_THROW(7);}

    friend constexpr bool operator==(const Z& x, const Z& y) {return x.i_ == y.i_;}
    friend void swap(Z&, Z&) {TEST_THROW(6);}
};

class W
{
    int i_;
public:
    constexpr W(int i) : i_(i) {}

    friend constexpr bool operator==(const W& x, const W& y) {return x.i_ == y.i_;}
    friend TEST_CONSTEXPR_CXX20 void swap(W& x, W& y) noexcept {std::swap(x.i_, y.i_);}
};

template<class T>
TEST_CONSTEXPR_CXX20 bool check_swap()
{
    {
        optional<T> opt1;
        optional<T> opt2;
        static_assert(noexcept(opt1.swap(opt2)) == true);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        opt1.swap(opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<T> opt1(1);
        optional<T> opt2;
        static_assert(noexcept(opt1.swap(opt2)) == true);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        opt1.swap(opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<T> opt1;
        optional<T> opt2(2);
        static_assert(noexcept(opt1.swap(opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        opt1.swap(opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<T> opt1(1);
        optional<T> opt2(2);
        static_assert(noexcept(opt1.swap(opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        opt1.swap(opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    return true;
}

int tc_utilities_optional_optional_object_optional_object_swap_swap(void) {
    check_swap<int>();
    check_swap<W>();
#if TEST_STD_VER > 17
    static_assert(check_swap<int>());
    static_assert(check_swap<W>());
#endif
    {
        optional<X> opt1;
        optional<X> opt2;
        static_assert(noexcept(opt1.swap(opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        opt1.swap(opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        TC_ASSERT_EXPR(X::dtor_called == 0);
    }
    {
        optional<X> opt1(1);
        optional<X> opt2;
        static_assert(noexcept(opt1.swap(opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        X::dtor_called = 0;
        opt1.swap(opt2);
        TC_ASSERT_EXPR(X::dtor_called == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<X> opt1;
        optional<X> opt2(2);
        static_assert(noexcept(opt1.swap(opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        X::dtor_called = 0;
        opt1.swap(opt2);
        TC_ASSERT_EXPR(X::dtor_called == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<X> opt1(1);
        optional<X> opt2(2);
        static_assert(noexcept(opt1.swap(opt2)) == true, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        X::dtor_called = 0;
        opt1.swap(opt2);
        TC_ASSERT_EXPR(X::dtor_called == 1);  // from inside std::swap
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<Y> opt1;
        optional<Y> opt2;
        static_assert(noexcept(opt1.swap(opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        opt1.swap(opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        TC_ASSERT_EXPR(Y::dtor_called == 0);
    }
    {
        optional<Y> opt1(1);
        optional<Y> opt2;
        static_assert(noexcept(opt1.swap(opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        Y::dtor_called = 0;
        opt1.swap(opt2);
        TC_ASSERT_EXPR(Y::dtor_called == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<Y> opt1;
        optional<Y> opt2(2);
        static_assert(noexcept(opt1.swap(opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        Y::dtor_called = 0;
        opt1.swap(opt2);
        TC_ASSERT_EXPR(Y::dtor_called == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
    {
        optional<Y> opt1(1);
        optional<Y> opt2(2);
        static_assert(noexcept(opt1.swap(opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        Y::dtor_called = 0;
        opt1.swap(opt2);
        TC_ASSERT_EXPR(Y::dtor_called == 0);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 2);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 1);
    }
    {
        optional<Z> opt1;
        optional<Z> opt2;
        static_assert(noexcept(opt1.swap(opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        opt1.swap(opt2);
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        optional<Z> opt1;
        opt1.emplace(1);
        optional<Z> opt2;
        static_assert(noexcept(opt1.swap(opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == false);
        try
        {
            opt1.swap(opt2);
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
        static_assert(noexcept(opt1.swap(opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == false);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        try
        {
            opt1.swap(opt2);
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
        static_assert(noexcept(opt1.swap(opt2)) == false, "");
        TC_ASSERT_EXPR(static_cast<bool>(opt1) == true);
        TC_ASSERT_EXPR(*opt1 == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt2) == true);
        TC_ASSERT_EXPR(*opt2 == 2);
        try
        {
            opt1.swap(opt2);
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
#endif

  return 0;
}
