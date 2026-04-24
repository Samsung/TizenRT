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

// class unsynchronized_pool_resource

#include <memory_resource>
#include <cassert>
#include <memory> // std::align

#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

bool is_aligned_to(void* p, std::size_t alignment) {
  void* p2     = p;
  std::size_t space = 1;
  void* result = std::align(alignment, 1, p2, space);
  return (result == p);
}

int tc_utilities_utility_mem_res_mem_res_pool_mem_res_pool_mem_unsync_allocate_overaligned_request(void) {
  globalMemCounter.reset();
  std::pmr::pool_options opts{1, 1024};
  auto unsync1                  = std::pmr::unsynchronized_pool_resource(opts, std::pmr::new_delete_resource());
  std::pmr::memory_resource& r1 = unsync1;

  constexpr std::size_t big_alignment = 8 * alignof(std::max_align_t);
  static_assert(big_alignment > 4);

  TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(0));

  void* ret = r1.allocate(2048, big_alignment);
  TC_ASSERT_EXPR(ret != nullptr);
  TC_ASSERT_EXPR(is_aligned_to(ret, big_alignment));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledGreaterThan(0));

  ret = r1.allocate(16, 4);
  TC_ASSERT_EXPR(ret != nullptr);
  TC_ASSERT_EXPR(is_aligned_to(ret, 4));
  ASSERT_WITH_LIBRARY_INTERNAL_ALLOCATIONS(globalMemCounter.checkNewCalledGreaterThan(1));

  return 0;
}
