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

// explicit bad_expected_access(E e);

// Effects: Initializes unex with std::move(e).

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>

#include "test_macros.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

// test explicit
static_assert(std::convertible_to<int, int>);
static_assert(!std::convertible_to<int, std::bad_expected_access<int>>);

int tc_utilities_expected_expected_bad_ctor_error(void) {
  std::bad_expected_access<MoveOnly> b(MoveOnly{3});
  TC_ASSERT_EXPR(b.error().get() == 3);

  return 0;
}
