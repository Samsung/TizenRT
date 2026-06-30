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

// class synchronized_pool_resource
// class unsynchronized_pool_resource

#include <memory_resource>
#include <cassert>
#include <new>
#include <type_traits>

#include "count_new.h"
#include "libcxx_tc_common.h"

class assert_on_compare : public std::pmr::memory_resource {
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

template <class PoolResource>
void test() {
  // Same type
  {
    PoolResource pr1;
    PoolResource pr2;
    TC_ASSERT_EXPR(pr1 == pr1);
    TC_ASSERT_EXPR(pr1 != pr2);

    std::pmr::memory_resource& mr1 = pr1;
    std::pmr::memory_resource& mr2 = pr2;
    TC_ASSERT_EXPR(mr1 == mr1);
    TC_ASSERT_EXPR(mr1 != mr2);
    TC_ASSERT_EXPR(mr1 == pr1);
    TC_ASSERT_EXPR(pr1 == mr1);
    TC_ASSERT_EXPR(mr1 != pr2);
    TC_ASSERT_EXPR(pr2 != mr1);
  }
  // Different types
  {
    PoolResource pr1;
    std::pmr::memory_resource& mr1 = pr1;
    assert_on_compare c;
    std::pmr::memory_resource& mr2 = c;
    TC_ASSERT_EXPR(mr1 != mr2);
    TC_ASSERT_EXPR(!(mr1 == mr2));
  }
}

int tc_utilities_utility_mem_res_mem_res_pool_mem_res_pool_mem_equality(void) {
  test<std::pmr::synchronized_pool_resource>();
  test<std::pmr::unsynchronized_pool_resource>();

  return 0;
}
