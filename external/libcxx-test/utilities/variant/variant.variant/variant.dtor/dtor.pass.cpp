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

// <variant>

// template <class ...Types> class variant;

// ~variant();

#include <cassert>
#include <type_traits>
#include <variant>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NonTDtor {
  static int count;
  NonTDtor() = default;
  ~NonTDtor() { ++count; }
};
int NonTDtor::count = 0;
static_assert(!std::is_trivially_destructible<NonTDtor>::value, "");

struct NonTDtor1 {
  static int count;
  NonTDtor1() = default;
  ~NonTDtor1() { ++count; }
};
int NonTDtor1::count = 0;
static_assert(!std::is_trivially_destructible<NonTDtor1>::value, "");

struct TDtor {
  TDtor(const TDtor &) {} // non-trivial copy
  ~TDtor() = default;
};
static_assert(!std::is_trivially_copy_constructible<TDtor>::value, "");
static_assert(std::is_trivially_destructible<TDtor>::value, "");

int tc_utilities_variant_variant_variant_variant_dtor_dtor(void) {
  {
    using V = std::variant<int, long, TDtor>;
    static_assert(std::is_trivially_destructible<V>::value, "");
  }
  {
    using V = std::variant<NonTDtor, int, NonTDtor1>;
    static_assert(!std::is_trivially_destructible<V>::value, "");
    {
      V v(std::in_place_index<0>);
      TC_ASSERT_EXPR(NonTDtor::count == 0);
      TC_ASSERT_EXPR(NonTDtor1::count == 0);
    }
    TC_ASSERT_EXPR(NonTDtor::count == 1);
    TC_ASSERT_EXPR(NonTDtor1::count == 0);
    NonTDtor::count = 0;
    { V v(std::in_place_index<1>); }
    TC_ASSERT_EXPR(NonTDtor::count == 0);
    TC_ASSERT_EXPR(NonTDtor1::count == 0);
    {
      V v(std::in_place_index<2>);
      TC_ASSERT_EXPR(NonTDtor::count == 0);
      TC_ASSERT_EXPR(NonTDtor1::count == 0);
    }
    TC_ASSERT_EXPR(NonTDtor::count == 0);
    TC_ASSERT_EXPR(NonTDtor1::count == 1);
  }

  return 0;
}
