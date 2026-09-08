//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// test_memory_resource requires RTTI for dynamic_cast
// UNSUPPORTED: no-rtti

// <memory_resource>

// polymorphic_allocator::allocate_object()
// polymorphic_allocator::deallocate_object()

#include <algorithm>
#include <cassert>
#include <concepts>
#include <memory_resource>

#include "tracking_mem_res.h"
#include "libcxx_tc_common.h"

template <class T>
void test() {
  std::size_t last_size      = 0;
  std::size_t last_alignment = 0;
  TrackingMemRes resource(&last_size, &last_alignment);

  std::pmr::polymorphic_allocator<T> allocator(&resource);

  {
    std::same_as<int*> decltype(auto) allocation = allocator.template allocate_object<int>();
    std::fill(allocation, allocation + 1, 3);
    TC_ASSERT_EXPR(last_size == sizeof(int));
    TC_ASSERT_EXPR(last_alignment == alignof(int));
    allocator.deallocate_object(allocation);
    TC_ASSERT_EXPR(last_size == sizeof(int));
    TC_ASSERT_EXPR(last_alignment == alignof(int));
  }
  {
    int* allocation = allocator.template allocate_object<int>(3);
    std::fill(allocation, allocation + 3, 3);
    TC_ASSERT_EXPR(last_size == sizeof(int) * 3);
    TC_ASSERT_EXPR(last_alignment == alignof(int));
    allocator.deallocate_object(allocation, 3);
    TC_ASSERT_EXPR(last_size == sizeof(int) * 3);
    TC_ASSERT_EXPR(last_alignment == alignof(int));
  }
}

struct S {};

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_allocate_deallocate_object(void) {
  test<std::byte>();
  test<S>();

  return 0;
}
