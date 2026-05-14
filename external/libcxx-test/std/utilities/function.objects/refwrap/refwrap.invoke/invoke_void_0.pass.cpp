//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
