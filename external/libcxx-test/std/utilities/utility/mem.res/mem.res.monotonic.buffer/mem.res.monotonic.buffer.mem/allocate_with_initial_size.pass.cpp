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
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// <memory_resource>

// class monotonic_buffer_resource

#include <memory_resource>
#include <cassert>

#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

void test(std::size_t initial_buffer_size) {
  globalMemCounter.reset();

  auto mono1 = std::pmr::monotonic_buffer_resource(initial_buffer_size, std::pmr::new_delete_resource());
  TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(0));

  (void)mono1.allocate(1, 1);
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkLastNewSizeGe(initial_buffer_size));

  (void)mono1.allocate(initial_buffer_size - 1, 1);
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
}

int tc_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_with_initial_size(void) {
  test(1);
  test(8);
  test(10);
  test(100);
  test(256);
  test(1000);
  test(1024);
  test(1000000);
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

  return 0;
}
