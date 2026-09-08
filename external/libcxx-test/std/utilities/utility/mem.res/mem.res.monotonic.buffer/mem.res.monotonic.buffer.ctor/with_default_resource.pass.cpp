//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// <memory_resource>

// class monotonic_buffer_resource

#include <memory_resource>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_ctor_with_default_resource(void) {
  std::pmr::memory_resource* expected = std::pmr::null_memory_resource();
  std::pmr::set_default_resource(expected);
  {
    char buffer[16];
    std::pmr::monotonic_buffer_resource r1;
    std::pmr::monotonic_buffer_resource r2(16);
    std::pmr::monotonic_buffer_resource r3(buffer, sizeof buffer);
    TC_ASSERT_EXPR(r1.upstream_resource() == expected);
    TC_ASSERT_EXPR(r2.upstream_resource() == expected);
    TC_ASSERT_EXPR(r3.upstream_resource() == expected);
  }

  expected = std::pmr::new_delete_resource();
  std::pmr::set_default_resource(expected);
  {
    char buffer[16];
    std::pmr::monotonic_buffer_resource r1;
    std::pmr::monotonic_buffer_resource r2(16);
    std::pmr::monotonic_buffer_resource r3(buffer, sizeof buffer);
    TC_ASSERT_EXPR(r1.upstream_resource() == expected);
    TC_ASSERT_EXPR(r2.upstream_resource() == expected);
    TC_ASSERT_EXPR(r3.upstream_resource() == expected);
  }

  return 0;
}
