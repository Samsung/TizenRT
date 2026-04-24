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
// UNSUPPORTED: c++03, c++11, c++14

// <functional>

// template <class T>
//   reference_wrapper(T&) -> reference_wrapper<T>;

#include <functional>
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_refwrap_refwrap_const_deduct(void) {
    int i = 0;
    std::reference_wrapper ri(i);
    static_assert(std::is_same_v<decltype(ri), std::reference_wrapper<int>>);
    std::reference_wrapper ri2(ri);
    static_assert(std::is_same_v<decltype(ri2), std::reference_wrapper<int>>);
    const int j = 0;
    std::reference_wrapper rj(j);
    static_assert(std::is_same_v<decltype(rj), std::reference_wrapper<const int>>);
    std::reference_wrapper rj2(rj);
    static_assert(std::is_same_v<decltype(rj2), std::reference_wrapper<const int>>);

    return 0;
}
