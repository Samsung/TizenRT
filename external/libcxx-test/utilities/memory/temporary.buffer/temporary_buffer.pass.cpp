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
