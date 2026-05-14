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

// template <class... UTypes> tuple(tuple<UTypes...>&& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <memory>
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

struct B
{
    int id_;

    explicit B(int i) : id_(i) {}

    virtual ~B() {}
};

struct D
    : B
{
    explicit D(int i) : B(i) {}
};

struct BonkersBananas {
  template <class T>
  operator T() &&;
  template <class T, class = void>
  explicit operator T() && = delete;
};

void test_bonkers_bananas_conversion() {
  using ReturnType = std::tuple<int, int>;
  static_assert(std::is_convertible<BonkersBananas, ReturnType>(), "");
  static_assert(!std::is_constructible<ReturnType, BonkersBananas>(), "");

}

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_convert_move(void) {
    {
        typedef std::tuple<long> T0;
        typedef std::tuple<long long> T1;
        T0 t0(2);
        T1 t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
    }
    {
        typedef std::tuple<long, char> T0;
        typedef std::tuple<long long, int> T1;
        T0 t0(2, 'a');
        T1 t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
    }
    {
        typedef std::tuple<long, char, D> T0;
        typedef std::tuple<long long, int, B> T1;
        T0 t0(2, 'a', D(3));
        T1 t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1).id_ == 3);
    }
    {
        D d(3);
        typedef std::tuple<long, char, D&> T0;
        typedef std::tuple<long long, int, B&> T1;
        T0 t0(2, 'a', d);
        T1 t1 = std::move(t0);
        d.id_ = 2;
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1).id_ == 2);
    }
    {
        typedef std::tuple<long, char, std::unique_ptr<D>> T0;
        typedef std::tuple<long long, int, std::unique_ptr<B>> T1;
        T0 t0(2, 'a', std::unique_ptr<D>(new D(3)));
        T1 t1 = std::move(t0);
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == int('a'));
        TC_ASSERT_EXPR(std::get<2>(t1)->id_ == 3);
    }
    {
        std::tuple<int> t1(42);
        std::tuple<Explicit> t2(std::move(t1));
        TC_ASSERT_EXPR(std::get<0>(t2).value == 42);
    }
    {
        std::tuple<int> t1(42);
        std::tuple<Implicit> t2 = std::move(t1);
        TC_ASSERT_EXPR(std::get<0>(t2).value == 42);
    }

  return 0;
}
