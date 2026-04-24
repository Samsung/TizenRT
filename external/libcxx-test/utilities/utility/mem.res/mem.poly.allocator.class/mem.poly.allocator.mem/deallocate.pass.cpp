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

// test_memory_resource requires RTTI for dynamic_cast
// UNSUPPORTED: no-rtti

// <memory_resource>

// template <class T> class polymorphic_allocator

// T* polymorphic_allocator<T>::deallocate(T*, size_t size)

#include <memory_resource>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

template <std::size_t S, size_t Align>
void testForSizeAndAlign() {
  struct T {
    alignas(Align) std::byte buf[S];
  };

  TestResource R;
  std::pmr::polymorphic_allocator<T> a(&R);

  for (int N = 1; N <= 5; ++N) {
    auto ret = a.allocate(N);
    TC_ASSERT_EXPR(R.checkAlloc(ret, N * sizeof(T), alignof(T)));

    a.deallocate(ret, N);
    TC_ASSERT_EXPR(R.checkDealloc(ret, N * sizeof(T), alignof(T)));

    R.reset();
  }
}

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_deallocate(void) {
  {
    std::pmr::polymorphic_allocator<int> a;
    ASSERT_SAME_TYPE(decltype(a.deallocate(nullptr, 0)), void);
  }
  {
    constexpr std::size_t MA = alignof(std::max_align_t);
    testForSizeAndAlign<1, 1>();
    testForSizeAndAlign<1, 2>();
    testForSizeAndAlign<1, MA>();
    testForSizeAndAlign<2, 2>();
    testForSizeAndAlign<73, alignof(void*)>();
    testForSizeAndAlign<73, MA>();
    testForSizeAndAlign<13, MA>();
  }

  return 0;
}
