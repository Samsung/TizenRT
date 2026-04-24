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

// class function<R(ArgTypes...)>

// explicit operator bool() const

#include <functional>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int g(int) {return 0;}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool(void) {
    static_assert(std::is_constructible<bool, std::function<void()> >::value, "");
    static_assert(!std::is_convertible<std::function<void()>, bool>::value, "");

    {
    std::function<int(int)> f;
    TC_ASSERT_EXPR(!f);
    f = g;
    TC_ASSERT_EXPR(f);
    }

  return 0;
}
