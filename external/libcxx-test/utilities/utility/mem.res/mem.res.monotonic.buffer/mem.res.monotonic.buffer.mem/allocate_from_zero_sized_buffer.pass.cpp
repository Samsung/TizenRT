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

int tc_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_from_zero_sized_buffer(void) {
  globalMemCounter.reset();
  {
    char buffer[100];
    auto mono1                    = std::pmr::monotonic_buffer_resource(buffer, 0, std::pmr::new_delete_resource());
    std::pmr::memory_resource& r1 = mono1;

    void* ret = r1.allocate(1, 1);
    TC_ASSERT_EXPR(ret != nullptr);
    ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
  }
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkDeleteCalledEq(1));
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

  globalMemCounter.reset();
  {
    auto mono1                    = std::pmr::monotonic_buffer_resource(nullptr, 0, std::pmr::new_delete_resource());
    std::pmr::memory_resource& r1 = mono1;

    void* ret = r1.allocate(1, 1);
    TC_ASSERT_EXPR(ret != nullptr);
    ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
  }
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkDeleteCalledEq(1));
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

  return 0;
}
