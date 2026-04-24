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
// UNSUPPORTED: c++03, c++11

// Make sure that we don't blow up the template instantiation recursion depth
// for tuples of size <= 1024.

#include <tuple>
#include <cassert>
#include <utility>
#include "libcxx_tc_common.h"

template <std::size_t... I>
constexpr void CreateTuple(std::index_sequence<I...>) {
  std::tuple<decltype(I)...> tuple(I...);
  TC_ASSERT_EXPR(std::get<0>(tuple) == 0);
  TC_ASSERT_EXPR(std::get<sizeof...(I)-1>(tuple) == sizeof...(I)-1);
}

constexpr bool test() {
  CreateTuple(std::make_index_sequence<1024>{});
  return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_recursion_depth(void) {
  test();
  static_assert(test(), "");
  return 0;
}
