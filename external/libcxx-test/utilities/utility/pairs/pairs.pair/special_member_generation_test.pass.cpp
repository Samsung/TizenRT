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
// UNSUPPORTED: c++03

// <utility>

// template <class T, class U> struct pair;

// pair(pair const&) = default;
// pair(pair &&) = default;
// pair& operator=(pair const&);
// pair& operator=(pair&&);

// Test that the copy/move constructors and assignment operators are
// correctly defined or deleted based on the properties of `T` and `U`.

#include <cassert>
#include <string>
#include <tuple>

#include "archetypes.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"
using namespace ImplicitTypes; // Get implicitly archetypes

namespace ConstructorTest {

template <class T1, bool CanCopy = true, bool CanMove = CanCopy> void test() {
  using P1 = std::pair<T1, int>;
  using P2 = std::pair<int, T1>;
  static_assert(std::is_copy_constructible<P1>::value == CanCopy, "");
  static_assert(std::is_move_constructible<P1>::value == CanMove, "");
  static_assert(std::is_copy_constructible<P2>::value == CanCopy, "");
  static_assert(std::is_move_constructible<P2>::value == CanMove, "");
};

} // namespace ConstructorTest

void test_constructors_exist() {
  using namespace ConstructorTest;
  {
    test<int>();
    test<int &>();
    test<int &&, false, true>();
    test<const int>();
    test<const int &>();
    test<const int &&, false, true>();
  }
  {
    test<Copyable>();
    test<Copyable &>();
    test<Copyable &&, false, true>();
  }
  {
    test<NonCopyable, false>();
    test<NonCopyable &, true>();
    test<NonCopyable &&, false, true>();
  }
  {
    // Even though CopyOnly has an explicitly deleted move constructor
    // pair's move constructor is only implicitly deleted and therefore
    // it doesn't participate in overload resolution.
    test<CopyOnly, true, true>();
    test<CopyOnly &, true>();
    test<CopyOnly &&, false, true>();
  }
  {
    test<MoveOnly, false, true>();
    test<MoveOnly &, true>();
    test<MoveOnly &&, false, true>();
  }
}

namespace AssignmentOperatorTest {

template <class T1, bool CanCopy = true, bool CanMove = CanCopy> void test() {
  using P1 = std::pair<T1, int>;
  using P2 = std::pair<int, T1>;
  static_assert(std::is_copy_assignable<P1>::value == CanCopy, "");
  static_assert(std::is_move_assignable<P1>::value == CanMove, "");
  static_assert(std::is_copy_assignable<P2>::value == CanCopy, "");
  static_assert(std::is_move_assignable<P2>::value == CanMove, "");
};

} // namespace AssignmentOperatorTest

void test_assignment_operator_exists() {
  using namespace AssignmentOperatorTest;
  {
    test<int>();
    test<int &>();
    test<int &&>();
    test<const int, false>();
    test<const int &, false>();
    test<const int &&, false>();
  }
  {
    test<Copyable>();
    test<Copyable &>();
    test<Copyable &&>();
  }
  {
    test<NonCopyable, false>();
    test<NonCopyable &, false>();
    test<NonCopyable &&, false>();
  }
  {
    test<CopyOnly, true>();
    test<CopyOnly &, true>();
    test<CopyOnly &&, true>();
  }
  {
    test<MoveOnly, false, true>();
    test<MoveOnly &, false, false>();
    test<MoveOnly &&, false, true>();
  }
}

int tc_utilities_utility_pairs_pairs_pair_special_member_generation_test(void) {
  test_constructors_exist();
  test_assignment_operator_exists();

  return 0;
}
