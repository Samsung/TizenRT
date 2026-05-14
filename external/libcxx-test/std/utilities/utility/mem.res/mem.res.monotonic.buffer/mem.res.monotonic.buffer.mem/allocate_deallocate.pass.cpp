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

int tc_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_deallocate(void) {
  {
    globalMemCounter.reset();

    auto mono1                    = std::pmr::monotonic_buffer_resource(std::pmr::new_delete_resource());
    std::pmr::memory_resource& r1 = mono1;

    void* ret = r1.allocate(50);
    TC_ASSERT_EXPR(ret);
    ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledGreaterThan(0));
    TC_ASSERT_EXPR(globalMemCounter.checkDeleteCalledEq(0));

    // mem.res.monotonic.buffer 1.2
    // A call to deallocate has no effect, thus the amount of memory
    // consumed increases monotonically until the resource is destroyed.
    // Check that deallocate is a no-op
    r1.deallocate(ret, 50);
    TC_ASSERT_EXPR(globalMemCounter.checkDeleteCalledEq(0));

    mono1.release();
    ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkDeleteCalledEq(1));
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

    globalMemCounter.reset();

    ret = r1.allocate(500);
    TC_ASSERT_EXPR(ret);
    ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledGreaterThan(0));
    TC_ASSERT_EXPR(globalMemCounter.checkDeleteCalledEq(0));

    // Check that the destructor calls release()
  }
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkDeleteCalledEq(1));
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

  return 0;
}
