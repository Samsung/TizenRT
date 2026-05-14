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

// template <class... Args> T& optional<T>::emplace(Args&&... args);

#include <optional>
#include <type_traits>
#include <cassert>
#include <memory>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"

using std::optional;

class X
{
    int i_;
    int j_ = 0;
public:
    constexpr X() : i_(0) {}
    constexpr X(int i) : i_(i) {}
    constexpr X(int i, int j) : i_(i), j_(j) {}

    friend constexpr bool operator==(const X& x, const X& y)
        {return x.i_ == y.i_ && x.j_ == y.j_;}
};

class Y
{
public:
    static bool dtor_called;
    Y() = default;
    Y(int) { TEST_THROW(6);}
    Y(const Y&) = default;
    Y& operator=(const Y&) = default;
    ~Y() {dtor_called = true;}
};

bool Y::dtor_called = false;

template <class T>
constexpr bool test_one_arg() {
    using Opt = std::optional<T>;
    {
        Opt opt;
        auto & v = opt.emplace();
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(0));
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        Opt opt;
        auto & v = opt.emplace(1);
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(1));
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        Opt opt(2);
        auto & v = opt.emplace();
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(0));
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        Opt opt(2);
        auto & v = opt.emplace(1);
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(1));
        TC_ASSERT_EXPR(&v == &*opt);
    }
    return true;
}


template <class T>
constexpr bool test_multi_arg()
{
    test_one_arg<T>();
    using Opt = std::optional<T>;
    {
        Opt opt;
        auto &v = opt.emplace(101, 41);
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(   v == T(101, 41));
        TC_ASSERT_EXPR(*opt == T(101, 41));
    }
    {
        Opt opt;
        auto &v = opt.emplace({1, 2, 3, 4});
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(  v == T(4)); // T sets its value to the size of the init list
        TC_ASSERT_EXPR(*opt == T(4));
    }
    {
        Opt opt;
        auto &v = opt.emplace({1, 2, 3, 4, 5}, 6);
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(  v == T(5)); // T sets its value to the size of the init list
        TC_ASSERT_EXPR(*opt == T(5)); // T sets its value to the size of the init list
    }
    return true;
}

template <class T>
void test_on_test_type() {

    T::reset();
    optional<T> opt;
    TC_ASSERT_EXPR(T::alive == 0);
    {
        T::reset_constructors();
        auto &v = opt.emplace();
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 1);
        TC_ASSERT_EXPR(T::default_constructed == 1);
        TC_ASSERT_EXPR(T::destroyed == 0);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T());
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        T::reset_constructors();
        auto &v = opt.emplace();
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 1);
        TC_ASSERT_EXPR(T::default_constructed == 1);
        TC_ASSERT_EXPR(T::destroyed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T());
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        T::reset_constructors();
        auto &v = opt.emplace(101);
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 1);
        TC_ASSERT_EXPR(T::value_constructed == 1);
        TC_ASSERT_EXPR(T::destroyed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(101));
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        T::reset_constructors();
        auto &v = opt.emplace(-10, 99);
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 1);
        TC_ASSERT_EXPR(T::value_constructed == 1);
        TC_ASSERT_EXPR(T::destroyed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(-10, 99));
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        T::reset_constructors();
        auto &v = opt.emplace(-10, 99);
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 1);
        TC_ASSERT_EXPR(T::value_constructed == 1);
        TC_ASSERT_EXPR(T::destroyed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(-10, 99));
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        T::reset_constructors();
        auto &v = opt.emplace({-10, 99, 42, 1});
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 1);
        TC_ASSERT_EXPR(T::value_constructed == 1);
        TC_ASSERT_EXPR(T::destroyed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(4)); // size of the initializer list
        TC_ASSERT_EXPR(&v == &*opt);
    }
    {
        T::reset_constructors();
        auto &v = opt.emplace({-10, 99, 42, 1}, 42);
        static_assert( std::is_same_v<T&, decltype(v)>, "" );
        TC_ASSERT_EXPR(T::alive == 1);
        TC_ASSERT_EXPR(T::constructed == 1);
        TC_ASSERT_EXPR(T::value_constructed == 1);
        TC_ASSERT_EXPR(T::destroyed == 1);
        TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
        TC_ASSERT_EXPR(*opt == T(4)); // size of the initializer list
        TC_ASSERT_EXPR(&v == &*opt);
    }
}

constexpr bool test_empty_emplace()
{
    optional<const int> opt;
    auto &v = opt.emplace(42);
    static_assert( std::is_same_v<const int&, decltype(v)>, "" );
    TC_ASSERT_EXPR(*opt == 42);
    TC_ASSERT_EXPR(   v == 42);
    opt.emplace();
    TC_ASSERT_EXPR(*opt == 0);
    return true;
}

int tc_utilities_optional_optional_object_optional_object_assign_emplace(void) {
    {
        test_on_test_type<TestTypes::TestType>();
        test_on_test_type<ExplicitTestTypes::TestType>();
    }
    {
        using T = int;
        test_one_arg<T>();
        test_one_arg<const T>();
#if TEST_STD_VER > 17
        static_assert(test_one_arg<T>());
        static_assert(test_one_arg<const T>());
#endif
    }
    {
        using T = ConstexprTestTypes::TestType;
        test_multi_arg<T>();
#if TEST_STD_VER > 17
        static_assert(test_multi_arg<T>());
#endif
    }
    {
        using T = ExplicitConstexprTestTypes::TestType;
        test_multi_arg<T>();
#if TEST_STD_VER > 17
        static_assert(test_multi_arg<T>());
#endif
    }
    {
        using T = TrivialTestTypes::TestType;
        test_multi_arg<T>();
#if TEST_STD_VER > 17
        static_assert(test_multi_arg<T>());
#endif
    }
    {
        using T = ExplicitTrivialTestTypes::TestType;
        test_multi_arg<T>();
#if TEST_STD_VER > 17
        static_assert(test_multi_arg<T>());
#endif
    }
    {
        test_empty_emplace();
#if TEST_STD_VER > 17
        static_assert(test_empty_emplace());
#endif
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    Y::dtor_called = false;
    {
        Y y;
        optional<Y> opt(y);
        try
        {
            TC_ASSERT_EXPR(static_cast<bool>(opt) == true);
            TC_ASSERT_EXPR(Y::dtor_called == false);
            auto &v = opt.emplace(1);
            static_assert( std::is_same_v<Y&, decltype(v)>, "" );
            TC_ASSERT_EXPR(false);
        }
        catch (int i)
        {
            TC_ASSERT_EXPR(i == 6);
            TC_ASSERT_EXPR(static_cast<bool>(opt) == false);
            TC_ASSERT_EXPR(Y::dtor_called == true);
        }
    }
#endif

  return 0;
}
