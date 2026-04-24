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

// 0 args, return int

int count = 0;

int f_int_0()
{
    return 3;
}

struct A_int_0
{
    int operator()() {return 4;}
};

void
test_int_0()
{
    // function
    {
    std::reference_wrapper<int ()> r1(f_int_0);
    TC_ASSERT_EXPR(r1() == 3);
    }
    // function pointer
    {
    int (*fp)() = f_int_0;
    std::reference_wrapper<int (*)()> r1(fp);
    TC_ASSERT_EXPR(r1() == 3);
    }
    // functor
    {
    A_int_0 a0;
    std::reference_wrapper<A_int_0> r1(a0);
    TC_ASSERT_EXPR(r1() == 4);
    }
}

// 1 arg, return void

void f_void_1(int i)
{
    count += i;
}

struct A_void_1
{
    void operator()(int i)
    {
        count += i;
    }
};

int tc_utilities_function_objects_refwrap_refwrap_invoke_invoke_int_0(void) {
    test_int_0();

  return 0;
}
