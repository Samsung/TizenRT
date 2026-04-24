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

// Class bad_function_call

// class bad_function_call
//     : public exception
// {
// public:
//   // 20.7.16.1.1, constructor:
//   bad_function_call();
// };

#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_func_wrap_func_wrap_badcall_bad_function_call(void) {
    static_assert((std::is_base_of<std::exception, std::bad_function_call>::value), "");

  return 0;
}
