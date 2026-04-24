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
// UNSUPPORTED: c++03, c++11, c++14, c++17
// TODO: Change to XFAIL once https://github.com/llvm/llvm-project/issues/40340 is fixed
// UNSUPPORTED: availability-pmr-missing

// test_memory_resource requires RTTI for dynamic_cast
// UNSUPPORTED: no-rtti

// <memory_resource>

// polymorphic_allocator::new_object()
// polymorphic_allocator::delete_object()

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
    std::same_as<int*> decltype(auto) allocation = allocator.template new_object<int>();
    std::fill(allocation, allocation + 1, 4);
    TC_ASSERT_EXPR(last_size == sizeof(int));
    TC_ASSERT_EXPR(last_alignment == alignof(int));
    allocator.delete_object(allocation);
  }
  {
    std::same_as<int*> decltype(auto) allocation = allocator.template new_object<int>(3);
    TC_ASSERT_EXPR(*allocation == 3);
    std::fill(allocation, allocation + 1, 4);
    TC_ASSERT_EXPR(last_size == sizeof(int));
    TC_ASSERT_EXPR(last_alignment == alignof(int));
    allocator.delete_object(allocation);
  }
  {
    struct TrackConstruction {
      bool* is_constructed_;
      TrackConstruction(bool* is_constructed) : is_constructed_(is_constructed) { *is_constructed = true; }
      ~TrackConstruction() { *is_constructed_ = false; }
    };

    bool is_constructed = false;

    std::same_as<TrackConstruction*> decltype(auto) allocation =
        allocator.template new_object<TrackConstruction>(&is_constructed);
    TC_ASSERT_EXPR(is_constructed);
    TC_ASSERT_EXPR(last_size == sizeof(TrackConstruction));
    TC_ASSERT_EXPR(last_alignment == alignof(TrackConstruction));
    allocator.delete_object(allocation);
    TC_ASSERT_EXPR(!is_constructed);
  }
}

struct S {};

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_new_delete_object(void) {
  test<std::byte>();
  test<S>();

  return 0;
}
