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

#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Incomplete;
template<class T> struct Holder { T t; };
typedef Holder<Incomplete> *Ptr;

Ptr no_args() { return nullptr; }
Ptr one_arg(Ptr p) { return p; }
Ptr two_args(Ptr p, Ptr) { return p; }
Ptr three_args(Ptr p, Ptr, Ptr) { return p; }

void one_arg_void(Ptr) { }

int tc_utilities_function_objects_refwrap_refwrap_invoke_robust_against_adl(void) {
    Ptr x = nullptr;
    const Ptr cx = nullptr;
    std::ref(no_args)();
    std::ref(one_arg)(x);
    std::ref(one_arg)(cx);
    std::ref(two_args)(x, x);
    std::ref(two_args)(x, cx);
    std::ref(two_args)(cx, x);
    std::ref(two_args)(cx, cx);
    std::ref(three_args)(x, x, x);
    std::ref(three_args)(x, x, cx);
    std::ref(three_args)(x, cx, x);
    std::ref(three_args)(cx, x, x);
    std::ref(three_args)(x, cx, cx);
    std::ref(three_args)(cx, x, cx);
    std::ref(three_args)(cx, cx, x);
    std::ref(three_args)(cx, cx, cx);
    std::ref(one_arg_void)(x);
    std::ref(one_arg_void)(cx);

    return 0;
}
