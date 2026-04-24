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

// reference_wrapper

// template <class... ArgTypes>
//   requires Callable<T, ArgTypes&&...>
//   Callable<T, ArgTypes&&...>::result_type
//   operator()(ArgTypes&&... args) const;

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// 0 args, return void

int count = 0;

void f_void_0()
{
    ++count;
}

struct A_void_0
{
    void operator()() {++count;}
};

void
test_void_0()
{
    int save_count = count;
    // function
    {
    std::reference_wrapper<void ()> r1(f_void_0);
    r1();
    TC_ASSERT_EXPR(count == save_count+1);
    save_count = count;
    }
    // function pointer
    {
    void (*fp)() = f_void_0;
    std::reference_wrapper<void (*)()> r1(fp);
    r1();
    TC_ASSERT_EXPR(count == save_count+1);
    save_count = count;
    }
    // functor
    {
    A_void_0 a0;
    std::reference_wrapper<A_void_0> r1(a0);
    r1();
    TC_ASSERT_EXPR(count == save_count+1);
    save_count = count;
    }
}

int tc_utilities_function_objects_refwrap_refwrap_invoke_invoke_void_0(void) {
    test_void_0();

  return 0;
}
