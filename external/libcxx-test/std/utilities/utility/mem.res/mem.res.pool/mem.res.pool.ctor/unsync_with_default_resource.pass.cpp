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
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_pool_mem_res_pool_ctor_unsync_with_default_resource(void) {
  std::pmr::memory_resource* expected = std::pmr::null_memory_resource();
  std::pmr::set_default_resource(expected);
  {
    std::pmr::pool_options opts{0, 0};
    std::pmr::unsynchronized_pool_resource r1;
    std::pmr::unsynchronized_pool_resource r2(opts);
    TC_ASSERT_EXPR(r1.upstream_resource() == expected);
    TC_ASSERT_EXPR(r2.upstream_resource() == expected);
  }

  expected = std::pmr::new_delete_resource();
  std::pmr::set_default_resource(expected);
  {
    std::pmr::pool_options opts{1024, 2048};
    std::pmr::unsynchronized_pool_resource r1;
    std::pmr::unsynchronized_pool_resource r2(opts);
    TC_ASSERT_EXPR(r1.upstream_resource() == expected);
    TC_ASSERT_EXPR(r2.upstream_resource() == expected);
  }

  return 0;
}
