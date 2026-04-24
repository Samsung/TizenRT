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

// template <class T1, class T2> struct pair

// pair& operator=(pair&& p);

#include <utility>
#include <memory>
#include <cassert>

#include "test_macros.h"
#include "archetypes.h"
#include "libcxx_tc_common.h"

struct CountAssign {
  int copied = 0;
  int moved = 0;
  TEST_CONSTEXPR_CXX20 CountAssign() = default;
  TEST_CONSTEXPR_CXX20 CountAssign& operator=(CountAssign const&) {
    ++copied;
    return *this;
  }
  TEST_CONSTEXPR_CXX20 CountAssign& operator=(CountAssign&&) {
    ++moved;
    return *this;
  }
};

struct NotAssignable {
  NotAssignable& operator=(NotAssignable const&) = delete;
  NotAssignable& operator=(NotAssignable&&) = delete;
};

struct MoveAssignable {
  MoveAssignable& operator=(MoveAssignable const&) = delete;
  MoveAssignable& operator=(MoveAssignable&&) = default;
};

struct CopyAssignable {
  CopyAssignable& operator=(CopyAssignable const&) = default;
  CopyAssignable& operator=(CopyAssignable&&) = delete;
};

TEST_CONSTEXPR_CXX20 bool test() {
  {
    typedef std::pair<ConstexprTestTypes::MoveOnly, int> P;
    P p1(3, 4);
    P p2;
    p2 = std::move(p1);
    TC_ASSERT_EXPR(p2.first.value == 3);
    TC_ASSERT_EXPR(p2.second == 4);
  }
  {
    using P = std::pair<int&, int&&>;
    int x = 42;
    int y = 101;
    int x2 = -1;
    int y2 = 300;
    P p1(x, std::move(y));
    P p2(x2, std::move(y2));
    p1 = std::move(p2);
    TC_ASSERT_EXPR(p1.first == x2);
    TC_ASSERT_EXPR(p1.second == y2);
  }
  {
    using P = std::pair<int, ConstexprTestTypes::DefaultOnly>;
    static_assert(!std::is_move_assignable<P>::value, "");
  }
  {
    // The move decays to the copy constructor
    using P = std::pair<CountAssign, ConstexprTestTypes::CopyOnly>;
    static_assert(std::is_move_assignable<P>::value, "");
    P p;
    P p2;
    p = std::move(p2);
    TC_ASSERT_EXPR(p.first.moved == 0);
    TC_ASSERT_EXPR(p.first.copied == 1);
    TC_ASSERT_EXPR(p2.first.moved == 0);
    TC_ASSERT_EXPR(p2.first.copied == 0);
  }
  {
    using P = std::pair<CountAssign, ConstexprTestTypes::MoveOnly>;
    static_assert(std::is_move_assignable<P>::value, "");
    P p;
    P p2;
    p = std::move(p2);
    TC_ASSERT_EXPR(p.first.moved == 1);
    TC_ASSERT_EXPR(p.first.copied == 0);
    TC_ASSERT_EXPR(p2.first.moved == 0);
    TC_ASSERT_EXPR(p2.first.copied == 0);
  }
  {
    using P1 = std::pair<int, NotAssignable>;
    using P2 = std::pair<NotAssignable, int>;
    using P3 = std::pair<NotAssignable, NotAssignable>;
    static_assert(!std::is_move_assignable<P1>::value, "");
    static_assert(!std::is_move_assignable<P2>::value, "");
    static_assert(!std::is_move_assignable<P3>::value, "");
  }
  {
    // We assign through the reference and don't move out of the incoming ref,
    // so this doesn't work (but would if the type were CopyAssignable).
    using P1 = std::pair<MoveAssignable&, int>;
    static_assert(!std::is_move_assignable<P1>::value, "");

    // ... works if it's CopyAssignable
    using P2 = std::pair<CopyAssignable&, int>;
    static_assert(std::is_move_assignable<P2>::value, "");

    // For rvalue-references, we can move-assign if the type is MoveAssignable
    // or CopyAssignable (since in the worst case the move will decay into a copy).
    using P3 = std::pair<MoveAssignable&&, int>;
    using P4 = std::pair<CopyAssignable&&, int>;
    static_assert(std::is_move_assignable<P3>::value, "");
    static_assert(std::is_move_assignable<P4>::value, "");

    // In all cases, we can't move-assign if the types are not assignable,
    // since we assign through the reference.
    using P5 = std::pair<NotAssignable&, int>;
    using P6 = std::pair<NotAssignable&&, int>;
    static_assert(!std::is_move_assignable<P5>::value, "");
    static_assert(!std::is_move_assignable<P6>::value, "");
  }
  return true;
}

int tc_utilities_utility_pairs_pairs_pair_assign_rv_pair(void) {
  test();
#if TEST_STD_VER >= 20
  static_assert(test());
#endif

  return 0;
}
