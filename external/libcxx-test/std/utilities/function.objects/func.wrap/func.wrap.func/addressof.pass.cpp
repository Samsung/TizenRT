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

// Make sure we can use std::function with a type that has a hostile overload
// of operator&().

#include <functional>
#include <cassert>

#include "operator_hijacker.h"
#include "libcxx_tc_common.h"

struct TrapAddressof : operator_hijacker {
    int operator()() const { return 1; }
};

int tc_utilities_function_objects_func_wrap_func_wrap_func_addressof(void) {
    std::function<int()> f = TrapAddressof();
    TC_ASSERT_EXPR(f() == 1);
    return 0;
}
