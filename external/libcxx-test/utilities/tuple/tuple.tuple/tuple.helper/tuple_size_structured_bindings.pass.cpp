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

// template <class... Types>
//   struct tuple_size<tuple<Types...>>
//     : public integral_constant<size_t, sizeof...(Types)> { };

// UNSUPPORTED: c++03, c++11, c++14

#include <tuple>
#include <array>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct S { int x; };

void test_decomp_user_type() {
  {
    S s{99};
    auto [m1] = s;
    auto& [r1] = s;
    TC_ASSERT_EXPR(m1 == 99);
    TC_ASSERT_EXPR(&r1 == &s.x);
  }
  {
    S const s{99};
    auto [m1] = s;
    auto& [r1] = s;
    TC_ASSERT_EXPR(m1 == 99);
    TC_ASSERT_EXPR(&r1 == &s.x);
  }
}

void test_decomp_tuple() {
  typedef std::tuple<int> T;
  {
    T s{99};
    auto [m1] = s;
    auto& [r1] = s;
    TC_ASSERT_EXPR(m1 == 99);
    TC_ASSERT_EXPR(&r1 == &std::get<0>(s));
  }
  {
    T const s{99};
    auto [m1] = s;
    auto& [r1] = s;
    TC_ASSERT_EXPR(m1 == 99);
    TC_ASSERT_EXPR(&r1 == &std::get<0>(s));
  }
}


void test_decomp_pair() {
  typedef std::pair<int, double> T;
  {
    T s{99, 42.5};
    auto [m1, m2] = s;
    auto& [r1, r2] = s;
    TC_ASSERT_EXPR(m1 == 99);
    TC_ASSERT_EXPR(m2 == 42.5);
    TC_ASSERT_EXPR(&r1 == &std::get<0>(s));
    TC_ASSERT_EXPR(&r2 == &std::get<1>(s));
  }
  {
    T const s{99, 42.5};
    auto [m1, m2] = s;
    auto& [r1, r2] = s;
    TC_ASSERT_EXPR(m1 == 99);
    TC_ASSERT_EXPR(m2 == 42.5);
    TC_ASSERT_EXPR(&r1 == &std::get<0>(s));
    TC_ASSERT_EXPR(&r2 == &std::get<1>(s));
  }
}

void test_decomp_array() {
  typedef std::array<int, 3> T;
  {
    T s{{99, 42, -1}};
    auto [m1, m2, m3] = s;
    auto& [r1, r2, r3] = s;
    TC_ASSERT_EXPR(m1 == 99);
    TC_ASSERT_EXPR(m2 == 42);
    TC_ASSERT_EXPR(m3 == -1);
    TC_ASSERT_EXPR(&r1 == &std::get<0>(s));
    TC_ASSERT_EXPR(&r2 == &std::get<1>(s));
    TC_ASSERT_EXPR(&r3 == &std::get<2>(s));
  }
  {
    T const s{{99, 42, -1}};
    auto [m1, m2, m3] = s;
    auto& [r1, r2, r3] = s;
    TC_ASSERT_EXPR(m1 == 99);
    TC_ASSERT_EXPR(m2 == 42);
    TC_ASSERT_EXPR(m3 == -1);
    TC_ASSERT_EXPR(&r1 == &std::get<0>(s));
    TC_ASSERT_EXPR(&r2 == &std::get<1>(s));
    TC_ASSERT_EXPR(&r3 == &std::get<2>(s));
  }
}

struct Test {
  int x;
};

template <std::size_t N>
int get(Test const&) { static_assert(N == 0, ""); return -1; }

template <>
struct std::tuple_element<0, Test> {
  typedef int type;
};

void test_before_tuple_size_specialization() {
  Test const t{99};
  auto& [p] = t;
  TC_ASSERT_EXPR(p == 99);
}

template <>
struct std::tuple_size<Test> {
public:
  static const std::size_t value = 1;
};

void test_after_tuple_size_specialization() {
  Test const t{99};
  auto& [p] = t;
  TC_ASSERT_EXPR(p == -1);
}

int tc_utilities_tuple_tuple_tuple_tuple_helper_tuple_size_structured_bindings(void) {
  test_decomp_user_type();
  test_decomp_tuple();
  test_decomp_pair();
  test_decomp_array();
  test_before_tuple_size_specialization();
  test_after_tuple_size_specialization();

  return 0;
}
