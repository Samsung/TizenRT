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
// <tuple>

// template <class... Types> class tuple;

// template <class... UTypes> tuple(const tuple<UTypes...>& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <utility>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Explicit {
  int value;
  explicit Explicit(int x) : value(x) {}
};

struct Implicit {
  int value;
  Implicit(int x) : value(x) {}
};

struct ExplicitTwo {
    ExplicitTwo() {}
    ExplicitTwo(ExplicitTwo const&) {}
    ExplicitTwo(ExplicitTwo &&) {}

    template <class T, class = typename std::enable_if<!std::is_same<T, ExplicitTwo>::value>::type>
    explicit ExplicitTwo(T) {}
};

struct B
{
    int id_;

    explicit B(int i) : id_(i) {}
};

struct D
    : B
{
    explicit D(int i) : B(i) {}
};

#if TEST_STD_VER > 11

struct A
{
    int id_;

    constexpr A(int i) : id_(i) {}
    friend constexpr bool operator==(const A& x, const A& y) {return x.id_ == y.id_;}
};

struct C
{
    int id_;

    constexpr explicit C(int i) : id_(i) {}
    friend constexpr bool operator==(const C& x, const C& y) {return x.id_ == y.id_;}
};

#endif

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_convert_copy(void) {
    {
        typedef std::tuple<long> T0;
        typedef std::tuple<long long> T1;
        T0 t0(2);
        T1 t1 = t0;
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
    }
#if TEST_STD_VER > 11
    {
        typedef std::tuple<int> T0;
        typedef std::tuple<A> T1;
        constexpr T0 t0(2);
        constexpr T1 t1 = t0;
        static_assert(std::get<0>(t1) == 2, "");
    }
    {
        typedef std::tuple<int> T0;
        typedef std::tuple<C> T1;
        constexpr T0 t0(2);
        constexpr T1 t1{t0};
        static_assert(std::get<0>(t1) == C(2), "");
    }
#endif
    {
        typedef std::tuple<long, char> T0;
        typedef std::tuple<long long, int> T1;
        T0 t0(2, 'a');
        T1 t1 = t0;
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
    }
    {
        typedef std::tuple<long, char, D> T0;
        typedef std::tuple<long long, int, B> T1;
        T0 t0(2, 'a', D(3));
        T1 t1 = t0;
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1).id_ == 3);
    }
    {
        D d(3);
        typedef std::tuple<long, char, D&> T0;
        typedef std::tuple<long long, int, B&> T1;
        T0 t0(2, 'a', d);
        T1 t1 = t0;
        d.id_ = 2;
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1).id_ == 2);
    }
    {
        typedef std::tuple<long, char, int> T0;
        typedef std::tuple<long long, int, B> T1;
        T0 t0(2, 'a', 3);
        T1 t1(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1).id_ == 3);
    }
    {
        const std::tuple<int> t1(42);
        std::tuple<Explicit> t2(t1);
        TC_ASSERT_EXPR(std::get<0>(t2).value == 42);
    }
    {
        const std::tuple<int> t1(42);
        std::tuple<Implicit> t2 = t1;
        TC_ASSERT_EXPR(std::get<0>(t2).value == 42);
    }
    {
        static_assert(std::is_convertible<ExplicitTwo&&, ExplicitTwo>::value, "");
        static_assert(std::is_convertible<std::tuple<ExplicitTwo&&>&&, const std::tuple<ExplicitTwo>&>::value, "");

        ExplicitTwo e;
        std::tuple<ExplicitTwo> t = std::tuple<ExplicitTwo&&>(std::move(e));
        ((void)t);
    }
  return 0;
}
