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

// T* polymorphic_allocator<T>::allocate(size_t n)

#include <memory_resource>
#include <cassert>
#include <exception>
#include <limits>
#include <memory>
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
    R.reset();
  }
}

#ifndef TEST_HAS_NO_EXCEPTIONS
template <std::size_t S>
void testAllocForSizeThrows() {
  struct T {
    std::byte buf[S];
  };

  using Alloc  = std::pmr::polymorphic_allocator<T>;
  using Traits = std::allocator_traits<Alloc>;
  NullResource R;
  Alloc a(&R);

  // Test that allocating exactly the max size does not throw.
  std::size_t maxSize = Traits::max_size(a);
  std::size_t sizeTypeMax = std::numeric_limits<std::size_t>::max();
  if (maxSize != sizeTypeMax) {
    // Test that allocating size_t(~0) throws bad alloc.
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      (void)a.allocate(sizeTypeMax);
      TC_ASSERT_EXPR(false);
    } catch (const std::exception&) {
    }
#endif // _LIBCPP_NO_EXCEPTIONS

    // Test that allocating even one more than the max size does throw.
    std::size_t overSize = maxSize + 1;
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      (void)a.allocate(overSize);
      TC_ASSERT_EXPR(false);
    } catch (const std::exception&) {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }
}
#endif // TEST_HAS_NO_EXCEPTIONS

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_mem_allocate(void) {
  {
    std::pmr::polymorphic_allocator<int> a;
    ASSERT_SAME_TYPE(decltype(a.allocate(0)), int*);
    static_assert(!noexcept(a.allocate(0)), "");
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
#ifndef TEST_HAS_NO_EXCEPTIONS
  {
    testAllocForSizeThrows<1>();
    testAllocForSizeThrows<2>();
    testAllocForSizeThrows<4>();
    testAllocForSizeThrows<8>();
    testAllocForSizeThrows<16>();
    testAllocForSizeThrows<73>();
    testAllocForSizeThrows<13>();
  }
#endif

  return 0;
}
