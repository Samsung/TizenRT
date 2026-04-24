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

// <functional>

// See https://llvm.org/PR20002

#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

using Fn = std::function<void()>;
struct S : public std::function<void()> { using function::function; };

int tc_utilities_function_objects_func_wrap_func_wrap_func_derive_from(void) {
    S s( [](){} );
    S f1( s );
#if TEST_STD_VER <= 14
    S f2(std::allocator_arg, std::allocator<int>{}, s);
#endif

  return 0;
}
