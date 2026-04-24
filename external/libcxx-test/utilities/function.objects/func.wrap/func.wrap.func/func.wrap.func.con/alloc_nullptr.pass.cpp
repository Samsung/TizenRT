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
// <functional>
// REQUIRES: c++11 || c++14

// class function<R(ArgTypes...)>

// template<class A> function(allocator_arg_t, const A&, nullptr_t);
//
// This signature was removed in C++17

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_alloc_nullptr(void) {
    std::function<int(int)> f(std::allocator_arg, bare_allocator<int>(), nullptr);
    TC_ASSERT_EXPR(!f);

  return 0;
}
