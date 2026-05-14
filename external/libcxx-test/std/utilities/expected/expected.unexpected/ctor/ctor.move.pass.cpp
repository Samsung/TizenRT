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

// constexpr unexpected(unexpected&&) = default;

#include <cassert>
#include <expected>
#include <utility>
#include "libcxx_tc_common.h"

struct Error {
  int i;
  constexpr Error(int ii) : i(ii) {}
  constexpr Error(Error&& other) : i(other.i) {other.i = 0;}
};

constexpr bool test() {
  std::unexpected<Error> unex(5);
  auto unex2 = std::move(unex);
  TC_ASSERT_EXPR(unex2.error().i == 5);
  TC_ASSERT_EXPR(unex.error().i == 0);
  return true;
}

int tc_utilities_expected_expected_unexpected_ctor_ctor_move(void) {
  test();
  static_assert(test());
  return 0;
}
