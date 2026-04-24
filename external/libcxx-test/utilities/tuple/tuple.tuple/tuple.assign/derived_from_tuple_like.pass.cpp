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

// template <class... UTypes>
//   tuple& operator=(const tuple<UTypes...>& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <array>
#include <string>
#include <utility>
#include <cassert>

#include "propagate_value_category.hpp"
#include "libcxx_tc_common.h"

struct TracksIntQuals {
  TracksIntQuals() : value(-1), value_category(VC_None), assigned(false) {}

  template <class Tp,
            class = typename std::enable_if<!std::is_same<
                typename std::decay<Tp>::type, TracksIntQuals>::value>::type>
  TracksIntQuals(Tp &&x)
      : value(x), value_category(getValueCategory<Tp &&>()), assigned(false) {
    static_assert(std::is_same<UnCVRef<Tp>, int>::value, "");
  }

  template <class Tp,
            class = typename std::enable_if<!std::is_same<
                typename std::decay<Tp>::type, TracksIntQuals>::value>::type>
  TracksIntQuals &operator=(Tp &&x) {
    static_assert(std::is_same<UnCVRef<Tp>, int>::value, "");
    value = x;
    value_category = getValueCategory<Tp &&>();
    assigned = true;
    return *this;
  }

  void reset() {
    value = -1;
    value_category = VC_None;
    assigned = false;
  }

  bool checkConstruct(int expect, ValueCategory expect_vc) const {
    return value != 1 && value == expect && value_category == expect_vc &&
           assigned == false;
  }

  bool checkAssign(int expect, ValueCategory expect_vc) const {
    return value != 1 && value == expect && value_category == expect_vc &&
           assigned == true;
  }

  int value;
  ValueCategory value_category;
  bool assigned;
};

template <class Tup>
struct DerivedFromTup : Tup {
  using Tup::Tup;
};

template <ValueCategory VC>
void do_derived_assign_test() {
  using Tup1 = std::tuple<long, TracksIntQuals>;
  Tup1 t;
  auto reset = [&]() {
    std::get<0>(t) = -1;
    std::get<1>(t).reset();
  };
  {
    DerivedFromTup<std::tuple<int, int>> d;
    std::get<0>(d) = 42;
    std::get<1>(d) = 101;

    t = ValueCategoryCast<VC>(d);
    TC_ASSERT_EXPR(std::get<0>(t) == 42);
    TC_ASSERT_EXPR(std::get<1>(t).checkAssign(101, VC));
  }
  reset();
  {
    DerivedFromTup<std::pair<int, int>> d;
    std::get<0>(d) = 42;
    std::get<1>(d) = 101;

    t = ValueCategoryCast<VC>(d);
    TC_ASSERT_EXPR(std::get<0>(t) == 42);
    TC_ASSERT_EXPR(std::get<1>(t).checkAssign(101, VC));
  }
  reset();
  {
#ifdef _LIBCPP_VERSION // assignment from std::array is a libc++ extension
    DerivedFromTup<std::array<int, 2>> d;
    std::get<0>(d) = 42;
    std::get<1>(d) = 101;

    t = ValueCategoryCast<VC>(d);
    TC_ASSERT_EXPR(std::get<0>(t) == 42);
    TC_ASSERT_EXPR(std::get<1>(t).checkAssign(101, VC));
#endif
  }
}

int tc_utilities_tuple_tuple_tuple_tuple_assign_derived_from_tuple_like(void) {
    do_derived_assign_test<VC_LVal | VC_Const>();
    do_derived_assign_test<VC_RVal>();

    return 0;
}
