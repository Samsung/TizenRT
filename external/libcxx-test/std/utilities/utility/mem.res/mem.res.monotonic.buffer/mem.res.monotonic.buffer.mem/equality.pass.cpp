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
#include <cstddef> // size_t
#include "libcxx_tc_common.h"

struct assert_on_compare : public std::pmr::memory_resource {
  void* do_allocate(std::size_t, size_t) override {
    TC_ASSERT_EXPR(false);
    return nullptr;
  }

  void do_deallocate(void*, std::size_t, size_t) override { TC_ASSERT_EXPR(false); }

  bool do_is_equal(const std::pmr::memory_resource&) const noexcept override {
    TC_ASSERT_EXPR(false);
    return true;
  }
};

int tc_utilities_utility_mem_res_mem_res_monotonic_buffer_mem_res_monotonic_buffer_mem_equality(void) {
  // Same object
  {
    std::pmr::monotonic_buffer_resource r1;
    std::pmr::monotonic_buffer_resource r2;
    TC_ASSERT_EXPR(r1 == r1);
    TC_ASSERT_EXPR(r1 != r2);

    std::pmr::memory_resource& p1 = r1;
    std::pmr::memory_resource& p2 = r2;
    TC_ASSERT_EXPR(p1 == p1);
    TC_ASSERT_EXPR(p1 != p2);
    TC_ASSERT_EXPR(p1 == r1);
    TC_ASSERT_EXPR(r1 == p1);
    TC_ASSERT_EXPR(p1 != r2);
    TC_ASSERT_EXPR(r2 != p1);
  }
  // Different types
  {
    std::pmr::monotonic_buffer_resource mono1;
    std::pmr::memory_resource& r1 = mono1;
    assert_on_compare c;
    std::pmr::memory_resource& r2 = c;
    TC_ASSERT_EXPR(r1 != r2);
    TC_ASSERT_EXPR(!(r1 == r2));
  }

  return 0;
}
