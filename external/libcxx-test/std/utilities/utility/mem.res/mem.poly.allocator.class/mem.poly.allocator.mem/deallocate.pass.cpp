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
