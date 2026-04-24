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
// REQUIRES: c++03 || c++11 || c++14
// binary_function was removed in C++17

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

// binary_function

#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_func_require_binary_function(void) {
    typedef std::binary_function<int, short, bool> bf;
    static_assert((std::is_same<bf::first_argument_type, int>::value), "");
    static_assert((std::is_same<bf::second_argument_type, short>::value), "");
    static_assert((std::is_same<bf::result_type, bool>::value), "");

  return 0;
}
