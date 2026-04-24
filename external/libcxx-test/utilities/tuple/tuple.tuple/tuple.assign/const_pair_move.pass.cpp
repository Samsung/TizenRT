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

// template<class U1, class U2>
// constexpr const tuple& operator=(pair<U1, U2>&& u) const;
//
// - sizeof...(Types) is 2,
// - is_assignable_v<const T1&, U1> is true, and
// - is_assignable_v<const T2&, U2> is true

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <tuple>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "copy_move_types.h"
#include "libcxx_tc_common.h"

// test constraints

// sizeof...(Types) != 2,
static_assert(std::is_assignable_v<const std::tuple<int&, int&>&, std::pair<int&, int&>&&>);
static_assert(!std::is_assignable_v<const std::tuple<int&>&, std::pair<int&, int&>&&>);
static_assert(!std::is_assignable_v<const std::tuple<int&, int&, int&>&, std::pair<int&, int&>&&>);

static_assert(std::is_assignable_v<const std::tuple<AssignableFrom<ConstMoveAssign>, ConstMoveAssign>&,
                                   std::pair<ConstMoveAssign, ConstMoveAssign>&&>);

// is_assignable_v<const T1&, U1> is false
static_assert(!std::is_assignable_v<const std::tuple<AssignableFrom<MoveAssign>, ConstMoveAssign>&,
                                    std::pair<MoveAssign, ConstMoveAssign>&&>);

// is_assignable_v<const T2&, U2> is false
static_assert(!std::is_assignable_v<const std::tuple<AssignableFrom<ConstMoveAssign>, AssignableFrom<MoveAssign>>&,
                                    std::tuple<ConstMoveAssign, MoveAssign>&&>);

constexpr bool test() {
  // reference types
  {
    int i1 = 1;
    int i2 = 2;
    long j1 = 3;
    long j2 = 4;
    std::pair<int&, int&> t1{i1, i2};
    const std::tuple<long&, long&> t2{j1, j2};
    t2 = std::move(t1);
    TC_ASSERT_EXPR(std::get<0>(t2) == 1);
    TC_ASSERT_EXPR(std::get<1>(t2) == 2);
  }

  // user defined const copy assignment
  {
    std::pair<ConstMoveAssign, ConstMoveAssign> t1{1, 2};
    const std::tuple<AssignableFrom<ConstMoveAssign>, ConstMoveAssign> t2{3, 4};
    t2 = std::move(t1);
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
    TC_ASSERT_EXPR(std::get<1>(t2).val == 2);
  }

  // make sure the right assignment operator of the type in the tuple is used
  {
    std::pair<TracedAssignment, TracedAssignment> t1{};
    const std::tuple<AssignableFrom<TracedAssignment>, AssignableFrom<TracedAssignment>> t2{};
    t2 = std::move(t1);
    TC_ASSERT_EXPR(std::get<0>(t2).v.constMoveAssign == 1);
    TC_ASSERT_EXPR(std::get<1>(t2).v.constMoveAssign == 1);
  }

  return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_assign_const_pair_move(void) {
  test();

// gcc cannot have mutable member in constant expression
#if !defined(TEST_COMPILER_GCC)
  static_assert(test());
#endif
  return 0;
}
