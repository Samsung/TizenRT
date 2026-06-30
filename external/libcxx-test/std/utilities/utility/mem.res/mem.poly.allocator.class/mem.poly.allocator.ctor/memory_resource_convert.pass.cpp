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

// polymorphic_allocator<T>::polymorphic_allocator(memory_resource *)

#include <memory_resource>
#include <type_traits>
#include <cassert>

#include "test_std_memory_resource.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_mem_res_mem_poly_allocator_class_mem_poly_allocator_ctor_memory_resource_convert(void) {
  {
    typedef std::pmr::polymorphic_allocator<void> A;
    static_assert(std::is_convertible_v<decltype(nullptr), A>);
    static_assert(std::is_convertible_v<std::pmr::memory_resource*, A>);
  }
  {
    typedef std::pmr::polymorphic_allocator<void> A;
    TestResource R;
    A const a(&R);
    TC_ASSERT_EXPR(a.resource() == &R);
  }

  return 0;
}
