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

// template <class T, class U> constexpr bool operator>(const optional<T>& x, const U& v);
// template <class T, class U> constexpr bool operator>(const U& v, const optional<T>& x);

#include <optional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using std::optional;

struct X {
  int i_;

  constexpr X(int i) : i_(i) {}
};

constexpr bool operator>(const X& lhs, const X& rhs) { return lhs.i_ > rhs.i_; }

int tc_utilities_optional_optional_comp_with_t_greater(void) {
  {
    typedef X T;
    typedef optional<T> O;

    constexpr T val(2);
    constexpr O o1;      // disengaged
    constexpr O o2{1};   // engaged
    constexpr O o3{val}; // engaged

    static_assert(!(o1 > T(1)), "");
    static_assert(!(o2 > T(1)), ""); // equal
    static_assert((o3 > T(1)), "");
    static_assert(!(o2 > val), "");
    static_assert(!(o3 > val), ""); // equal
    static_assert(!(o3 > T(3)), "");

    static_assert((T(1) > o1), "");
    static_assert(!(T(1) > o2), ""); // equal
    static_assert(!(T(1) > o3), "");
    static_assert((val > o2), "");
    static_assert(!(val > o3), ""); // equal
    static_assert((T(3) > o3), "");
  }
  {
    using O = optional<int>;
    constexpr O o1(42);
    static_assert(o1 > 11l, "");
    static_assert(!(42l > o1), "");
  }
  {
    using O = optional<const int>;
    constexpr O o1(42);
    static_assert(o1 > 11, "");
    static_assert(!(42 > o1), "");
  }

  return 0;
}
