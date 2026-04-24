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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <functional>
//
// reference_wrapper
//
// template <ObjectType T> reference_wrapper<T> ref(T& t);
//
//  where T is an incomplete type (since C++20)

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"


struct Foo;

Foo& get_foo();

void test() {
    Foo& foo = get_foo();
    std::reference_wrapper<Foo> ref = std::ref(foo);
    TC_ASSERT_EXPR(&ref.get() == &foo);
}

struct Foo { };

Foo& get_foo() {
    static Foo foo;
    return foo;
}

int tc_utilities_function_objects_refwrap_refwrap_helpers_ref_incomplete(void) {
    test();
    return 0;
}
