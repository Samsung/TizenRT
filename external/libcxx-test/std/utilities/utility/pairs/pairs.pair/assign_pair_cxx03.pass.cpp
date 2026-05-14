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
// REQUIRES: c++03

// <utility>

// template <class T1, class T2> struct pair

// pair& operator=(pair const& p);

#include <utility>
#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NonAssignable {
  NonAssignable() {}
private:
  NonAssignable& operator=(NonAssignable const&);
};

struct Incomplete;
extern Incomplete inc_obj;

struct ConstructibleFromInt {
  ConstructibleFromInt() : value(-1) { }
  explicit ConstructibleFromInt(int v) : value(v) { }
  int value;
};

int tc_utilities_utility_pairs_pairs_pair_assign_pair_cxx03(void) {
    {
      // Test that we don't constrain the assignment operator in C++03 mode.
      // Since we don't have access control SFINAE having pair evaluate SFINAE
      // may cause a hard error.
      typedef std::pair<int, NonAssignable> P;
      static_assert(std::is_copy_assignable<P>::value, "");
    }
    {
      typedef std::pair<int, Incomplete&> P;
      static_assert(std::is_copy_assignable<P>::value, "");
      P p(42, inc_obj);
      TC_ASSERT_EXPR(&p.second == &inc_obj);
    }
    {
      // The type is constructible from int, but not assignable from int.
      // This ensures that operator=(pair const&) can be used in conjunction with
      // pair(pair<U, V> const&) to mimic operator=(pair<U, V> const&) in C++03.
      // This is weird but valid in C++03.
      std::pair<ConstructibleFromInt, char> p;
      std::pair<int, char> from(11, 'x');
      p = from;
      TC_ASSERT_EXPR(p.first.value == 11);
      TC_ASSERT_EXPR(p.second == 'x');
    }

  return 0;
}

struct Incomplete {};
Incomplete inc_obj;
