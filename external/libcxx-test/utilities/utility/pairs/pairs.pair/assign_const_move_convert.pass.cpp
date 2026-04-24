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
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// <utility>

// template <class T1, class T2> struct pair
// template <class U1, class U2>
// constexpr const pair& operator=(pair<U1, U2>&& p) const;

#include <cassert>
#include <utility>

#include "test_macros.h"
#include "copy_move_types.h"
#include "libcxx_tc_common.h"

// Constraints:
// is_assignable<const first_type&, U1> is true and
// is_assignable<const second_type&, U2> is true.

// clang-format off
static_assert( std::is_assignable_v<const std::pair<int&&, int&&>&,
                                    std::pair<long&&, long&&>&&>);
static_assert(!std::is_assignable_v<const std::pair<int, int>&,
                                    std::pair<long, long>&&>);
static_assert(!std::is_assignable_v<const std::pair<int, int&&>&,
                                    std::pair<long, long&&>&&>);
static_assert(!std::is_assignable_v<const std::pair<int&&, int>&,
                                    std::pair<long&&, long>&&>);

static_assert(std::is_assignable_v<
    const std::pair<AssignableFrom<ConstMoveAssign>, AssignableFrom<ConstMoveAssign>>&,
    std::pair<ConstMoveAssign, ConstMoveAssign>&&>);

static_assert(!std::is_assignable_v<
    const std::pair<AssignableFrom<MoveAssign>, AssignableFrom<MoveAssign>>&,
    std::pair<MoveAssign, MoveAssign>&&>);
// clang-format on

constexpr bool test() {
  // reference types
  {
    int i1  = 1;
    int i2  = 2;
    long j1 = 3;
    long j2 = 4;
    std::pair<int&&, int&&> p1{std::move(i1), std::move(i2)};
    const std::pair<long&&, long&&> p2{std::move(j1), std::move(j2)};
    p2 = std::move(p1);
    TC_ASSERT_EXPR(p2.first == 1);
    TC_ASSERT_EXPR(p2.second == 2);
  }

  // user defined const move assignment
  {
    std::pair<ConstMoveAssign, ConstMoveAssign> p1{1, 2};
    const std::pair<AssignableFrom<ConstMoveAssign>, AssignableFrom<ConstMoveAssign>> p2{3, 4};
    p2 = std::move(p1);
    TC_ASSERT_EXPR(p2.first.v.val == 1);
    TC_ASSERT_EXPR(p2.second.v.val == 2);
  }

  // The correct assignment operator of the underlying type is used
  {
    std::pair<TracedAssignment, TracedAssignment> t1{};
    const std::pair<AssignableFrom<TracedAssignment>, AssignableFrom<TracedAssignment>> t2{};
    t2 = std::move(t1);
    TC_ASSERT_EXPR(t2.first.v.constMoveAssign == 1);
    TC_ASSERT_EXPR(t2.second.v.constMoveAssign == 1);
  }

  return true;
}

int tc_utilities_utility_pairs_pairs_pair_assign_const_move_convert(void) {
  test();
// gcc cannot have mutable member in constant expression
#if !defined(TEST_COMPILER_GCC)
  static_assert(test());
#endif
  return 0;
}
