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
// <optional>

// template <class T, class U> constexpr bool operator<= (const optional<T>& x, const optional<U>& y);

#include <optional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

struct X {
  int i_;

  constexpr X(int i) : i_(i) {}
};

constexpr bool operator<=(const X& lhs, const X& rhs) {
  return lhs.i_ <= rhs.i_;
}

int tc_utilities_optional_optional_relops_less_equal(void) {
  {
    typedef optional<X> O;

    constexpr O o1;    // disengaged
    constexpr O o2;    // disengaged
    constexpr O o3{1}; // engaged
    constexpr O o4{2}; // engaged
    constexpr O o5{1}; // engaged

    static_assert((o1 <= o1), "");
    static_assert((o1 <= o2), "");
    static_assert((o1 <= o3), "");
    static_assert((o1 <= o4), "");
    static_assert((o1 <= o5), "");

    static_assert((o2 <= o1), "");
    static_assert((o2 <= o2), "");
    static_assert((o2 <= o3), "");
    static_assert((o2 <= o4), "");
    static_assert((o2 <= o5), "");

    static_assert(!(o3 <= o1), "");
    static_assert(!(o3 <= o2), "");
    static_assert((o3 <= o3), "");
    static_assert((o3 <= o4), "");
    static_assert((o3 <= o5), "");

    static_assert(!(o4 <= o1), "");
    static_assert(!(o4 <= o2), "");
    static_assert(!(o4 <= o3), "");
    static_assert((o4 <= o4), "");
    static_assert(!(o4 <= o5), "");

    static_assert(!(o5 <= o1), "");
    static_assert(!(o5 <= o2), "");
    static_assert((o5 <= o3), "");
    static_assert((o5 <= o4), "");
    static_assert((o5 <= o5), "");
  }
  {
    using O1 = optional<int>;
    using O2 = optional<long>;
    constexpr O1 o1(42);
    static_assert(o1 <= O2(42), "");
    static_assert(!(O2(101) <= o1), "");
  }
  {
    using O1 = optional<int>;
    using O2 = optional<const int>;
    constexpr O1 o1(42);
    static_assert(o1 <= O2(42), "");
    static_assert(!(O2(101) <= o1), "");
  }

  return 0;
}
