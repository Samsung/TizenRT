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

#include "test_macros.h"
#include "count_new.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_allocate_overaligned_request(void) {
  globalMemCounter.reset();
  auto mono1                    = std::pmr::monotonic_buffer_resource(1024, std::pmr::new_delete_resource());
  std::pmr::memory_resource& r1 = mono1;

  constexpr std::size_t big_alignment = 8 * alignof(std::max_align_t);
  static_assert(big_alignment > 4);

  void* ret = r1.allocate(2048, big_alignment);
  TC_ASSERT_EXPR(ret != nullptr);
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(1));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkAlignedNewCalledEq(1));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkLastNewSizeGe(2048));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkLastNewAlignEq(big_alignment));

  // Check that a single highly aligned allocation request doesn't
  // permanently "poison" the resource to allocate only super-aligned
  // blocks of memory.
  ret = r1.allocate(globalMemCounter.last_new_size, 4);
  TC_ASSERT_EXPR(ret != nullptr);
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledEq(2));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkAlignedNewCalledEq(1));

  return 0;
}
