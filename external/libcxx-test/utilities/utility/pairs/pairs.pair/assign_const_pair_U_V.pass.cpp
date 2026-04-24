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
// <utility>

// template <class T1, class T2> struct pair

// template<class U, class V> pair& operator=(const pair<U, V>& p);

#include <utility>
#include <cassert>

#include "test_macros.h"
#if TEST_STD_VER >= 11
#include "archetypes.h"
#include "libcxx_tc_common.h"
#endif

struct CopyAssignableInt {
  CopyAssignableInt& operator=(int&) { return *this; }
};

struct Unrelated {};

TEST_CONSTEXPR_CXX20 bool test() {
  {
    typedef std::pair<int, short> P1;
    typedef std::pair<double, long> P2;
    P1 p1(3, static_cast<short>(4));
    P2 p2;
    p2 = p1;
    TC_ASSERT_EXPR(p2.first == 3);
    TC_ASSERT_EXPR(p2.second == 4);
  }
#if TEST_STD_VER >= 20
  {
    using C = ConstexprTestTypes::TestType;
    using P = std::pair<int, C>;
    using T = std::pair<long, C>;
    const T t(42, -42);
    P p(101, 101);
    p = t;
    TC_ASSERT_EXPR(p.first == 42);
    TC_ASSERT_EXPR(p.second.value == -42);
  }
#elif TEST_STD_VER >= 11
  {
    using C = TestTypes::TestType;
    using P = std::pair<int, C>;
    using T = std::pair<long, C>;
    const T t(42, -42);
    P p(101, 101);
    C::reset_constructors();
    p = t;
    TC_ASSERT_EXPR(C::constructed == 0);
    TC_ASSERT_EXPR(C::assigned == 1);
    TC_ASSERT_EXPR(C::copy_assigned == 1);
    TC_ASSERT_EXPR(C::move_assigned == 0);
    TC_ASSERT_EXPR(p.first == 42);
    TC_ASSERT_EXPR(p.second.value == -42);
  }
  { // test const requirement
    using T = std::pair<CopyAssignableInt, CopyAssignableInt>;
    using P = std::pair<int, int>;
    static_assert(!std::is_assignable<T&, P const>::value, "");
  }
  {
    using T = std::pair<int, Unrelated>;
    using P = std::pair<Unrelated, int>;
    static_assert(!std::is_assignable<T&, P&>::value, "");
    static_assert(!std::is_assignable<P&, T&>::value, "");
  }
#endif
#if TEST_STD_VER >= 11 || defined(_LIBCPP_VERSION) // valid in C++11, provided in C++03 with libc++ as an extension
  {
    int i = 0, j = 0;
    std::pair<int&, int&> p(i, j);
    const std::pair<const int, const int> from(11, 12);
    p = from;
    TC_ASSERT_EXPR(i == 11);
    TC_ASSERT_EXPR(j == 12);
  }
#endif
  return true;
}

int tc_utilities_utility_pairs_pairs_pair_assign_const_pair_U_V(void) {
  test();
#if TEST_STD_VER >= 20
  static_assert(test());
#endif

  return 0;
}
