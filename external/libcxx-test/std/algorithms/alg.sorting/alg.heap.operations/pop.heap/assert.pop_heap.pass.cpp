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
// REQUIRES: has-unix-headers
// UNSUPPORTED: c++03
// UNSUPPORTED: !libcpp-has-debug-mode && !libcpp-has-assertions
// XFAIL: availability-verbose_abort-missing

// <algorithm>

// Calling `pop_heap` on an empty range is invalid.

#include <algorithm>

#include <array>
#include "check_assertion.h"
#include "libcxx_tc_common.h"

int tc_algorithms_alg_sorting_alg_heap_operations_pop_heap_assert_pop_heap(void) {
  std::array<int, 0> a;

  TEST_LIBCPP_ASSERT_FAILURE(std::pop_heap(a.begin(), a.end()), "The heap given to pop_heap must be non-empty");

  return 0;
}
