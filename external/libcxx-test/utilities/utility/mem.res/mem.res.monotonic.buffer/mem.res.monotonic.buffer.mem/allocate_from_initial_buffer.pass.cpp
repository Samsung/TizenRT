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

int tc_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_from_initial_buffer(void) {
  globalMemCounter.reset();
  char buffer[100];
  auto mono1 = std::pmr::monotonic_buffer_resource(buffer, sizeof buffer, std::pmr::new_delete_resource());
  std::pmr::memory_resource& r1 = mono1;

  // Check that construction with a buffer does not allocate anything from the upstream
  TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(0));

  // Check that an allocation that fits in the buffer does not allocate anything from the upstream
  void* ret = r1.allocate(50);
  TC_ASSERT_EXPR(ret);
  TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(0));

  // Check a second allocation
  ret = r1.allocate(20);
  TC_ASSERT_EXPR(ret);
  TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(0));

  r1.deallocate(ret, 50);
  TC_ASSERT_EXPR(globalMemCounter.checkDeleteCalledEq(0));

  // Check an allocation that doesn't fit in the original buffer
  ret = r1.allocate(50);
  TC_ASSERT_EXPR(ret);
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));

  r1.deallocate(ret, 50);
  TC_ASSERT_EXPR(globalMemCounter.checkDeleteCalledEq(0));

  mono1.release();
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkDeleteCalledEq(1));
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

  return 0;
}
