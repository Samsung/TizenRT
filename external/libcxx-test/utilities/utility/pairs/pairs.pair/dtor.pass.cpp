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

// ~pair()

// C++17 added:
//   The destructor of pair shall be a trivial destructor
//     if (is_trivially_destructible_v<T1> && is_trivially_destructible_v<T2>) is true.


#include <utility>
#include <type_traits>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_pairs_pairs_pair_dtor(void) {
  static_assert((std::is_trivially_destructible<
      std::pair<int, float> >::value), "");
  static_assert((!std::is_trivially_destructible<
      std::pair<int, std::string> >::value), "");

  return 0;
}
