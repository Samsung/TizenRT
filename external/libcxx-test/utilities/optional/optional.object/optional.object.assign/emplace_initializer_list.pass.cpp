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

// template <class U, class... Args>
//   T& optional<T>::emplace(initializer_list<U> il, Args&&... args);

#include <optional>
#include <type_traits>
#include <cassert>
#include <vector>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

class X
{
    int i_;
    int j_ = 0;
    bool* dtor_called_;
public:
    constexpr X(bool& dtor_called) : i_(0), dtor_called_(&dtor_called) {}
    constexpr X(int i, bool& dtor_called) : i_(i), dtor_called_(&dtor_called) {}
    constexpr X(std::initializer_list<int> il, bool& dtor_called)
    : i_(il.begin()[0]), j_(il.begin()[1]), dtor_called_(&dtor_called) {}
    X(const X&) = default;
    X& operator=(const X&) = default;
    TEST_CONSTEXPR_CXX20 ~X() {*dtor_called_ = true;}

    friend constexpr bool operator==(const X& x, const X& y)
        {return x.i_ == y.i_ && x.j_ == y.j_;}
};

class Y
{
    int i_;
    int j_ = 0;
public:
    constexpr Y() : i_(0) {}
    constexpr Y(int i) : i_(i) {}
    constexpr Y(std::initializer_list<int> il) : i_(il.begin()[0]), j_(il.begin()[1]) {}

    friend constexpr bool operator==(const Y& x, const Y& y)
        {return x.i_ == y.i_ && x.j_ == y.j_;}
};

class Z
{
    int i_;
    int j_ = 0;
public:
    static bool dtor_called;
    Z() : i_(0) {}
    Z(int i) : i_(i) {}
    Z(std::initializer_list<int> il) : i_(il.begin()[0]), j_(il.begin()[1])
        { TEST_THROW(6);}
    Z(const Z&) = default;
    Z& operator=(const Z&) = default;
    ~Z() {dtor_called = true;}

    friend bool operator==(const Z& x, const Z& y)
        {return x.i_ == y.i_ && x.j_ == y.j_;}
};

bool Z::dtor_called = false;

TEST_CONSTEXPR_CXX20 bool check_X()
{
    bool dtor_called = false;
    X x(dtor_called);
    optional<X> opt(x);
    TC_ASSERT_EXPR(dtor_called == false);
    auto &v = opt.emplace({1, 2}, dtor_called);
    static_assert( std::is_same_v<X&, decltype(v)>, "" );
    TC_ASSERT_EXPR(dtor_called);
    TC_ASSERT_EXPR(*opt == X({1, 2}, dtor_called));
    TC_ASSERT_EXPR(&v == &*opt);
    return true;
}

TEST_CONSTEXPR_CXX20 bool check_Y()
{
    optional<Y> opt;
    auto &v = opt.emplace({1, 2});
    static_assert( std::is_same_v<Y&, decltype(v)>, "" );
    TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
    TC_ASSERT_EXPR(*opt == Y({1, 2}));
    TC_ASSERT_EXPR(&v == &*opt);
    return true;
}

int tc_utilities_optional_optional_object_optional_object_assign_emplace_initializer_list(void) {
    {
        check_X();
#if TEST_STD_VER > 17
        static_assert(check_X());
#endif
    }
    {
        optional<std::vector<int>> opt;
        auto &v = opt.emplace({1, 2, 3}, std::allocator<int>());
        static_assert( std::is_same_v<std::vector<int>&, decltype(v)>, "" );
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == std::vector<int>({1, 2, 3}));
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        check_Y();
#if TEST_STD_VER > 17
        static_assert(check_Y());
#endif
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        Z z;
        optional<Z> opt(z);
        try
        {
            TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
            TC_ASSERT_EXPR(Z::dtor_called == false);
            auto &v = opt.emplace({1, 2});
            static_assert( std::is_same_v<Z&, decltype(v)>, "" );
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 6);
            TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
            TC_ASSERT_EXPR(Z::dtor_called == true);
        }
    }
#endif

  return 0;
}
