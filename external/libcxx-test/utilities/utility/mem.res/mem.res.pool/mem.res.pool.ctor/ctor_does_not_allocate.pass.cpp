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

// class synchronized_pool_resource
// class unsynchronized_pool_resource

#include <memory_resource>
#include <cassert>

#include "count_new.h"
#include "libcxx_tc_common.h"

template <class PoolResource>
void test() {
  // Constructing a pool resource should not cause allocations
  // by itself; the resource should wait to allocate until an
  // allocation is requested.

  globalMemCounter.reset();
  std::pmr::set_default_resource(std::pmr::new_delete_resource());

  PoolResource r1;
  TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(0));

  PoolResource r2(std::pmr::pool_options{1024, 2048});
  TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(0));

  PoolResource r3(std::pmr::pool_options{1024, 2048}, std::pmr::new_delete_resource());
  TC_ASSERT_EXPR(globalMemCounter.checkNewCalledEq(0));
}

int tc_utilities_utility_mem_res_mem_res_pool_mem_res_pool_ctor_ctor_does_not_allocate(void) {
  test<std::pmr::unsynchronized_pool_resource>();
  test<std::pmr::synchronized_pool_resource>();

  return 0;
}
