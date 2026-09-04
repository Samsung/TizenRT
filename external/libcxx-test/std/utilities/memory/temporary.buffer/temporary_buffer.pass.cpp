//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// template <class T>
//   pair<T*, ptrdiff_t>
//   get_temporary_buffer(ptrdiff_t n);
//
// template <class T>
//   void
//   return_temporary_buffer(T* p);

#include <memory>
#include <cassert>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_memory_temporary_buffer_temporary_buffer(void) {
    std::pair<int*, std::ptrdiff_t> ip = std::get_temporary_buffer<int>(5);
    TC_ASSERT_EXPR(ip.first);
    TC_ASSERT_EXPR(ip.second == 5);
    std::return_temporary_buffer(ip.first);

  return 0;
}
