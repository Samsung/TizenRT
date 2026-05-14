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
// UNSUPPORTED: c++03

// Aligned allocations are not supported on macOS < 10.13
// Note: use 'unsupported' instead of 'xfail' to ensure
// we won't pass prior to c++17.
// UNSUPPORTED: stdlib=apple-libc++ && target={{.+}}-apple-macosx10.{{9|10|11|12}}

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// <memory>

// template <class T>
//   pair<T*, ptrdiff_t>
//   get_temporary_buffer(ptrdiff_t n);
//
// template <class T>
//   void
//   return_temporary_buffer(T* p);

#include <cassert>
#include <cstdint>
#include <memory>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct alignas(32) A {
    int field;
};

int tc_utilities_memory_temporary_buffer_overaligned(void) {
    std::pair<A*, std::ptrdiff_t> ip = std::get_temporary_buffer<A>(5);
    TC_ASSERT_EXPR(!(ip.first == nullptr) ^ (ip.second == 0));
    TC_ASSERT_EXPR(reinterpret_cast<std::uintptr_t>(ip.first) % alignof(A) == 0);
    std::return_temporary_buffer(ip.first);

  return 0;
}
