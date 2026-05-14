//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <functional>

// class function<R(ArgTypes...)>

// template<class F, class A> void assign(F&&, const A&);
//     This call was removed post-C++14

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

class A
{
    int data_[10];
public:
    static int count;

    A()
    {
        ++count;
        for (int i = 0; i < 10; ++i)
            data_[i] = i;
    }

    A(const A&) {++count;}

    ~A() {--count;}

    int operator()(int i) const
    {
        for (int j = 0; j < 10; ++j)
            i += data_[j];
        return i;
    }

    int foo(int) const {return 1;}
};

int A::count = 0;

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_mod_assign_F_alloc(void) {
#if TEST_STD_VER <= 14
    {
    std::function<int(int)> f;
    f.assign(A(), test_allocator<A>());
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(f.target<A>());
    TC_ASSERT_EXPR(f.target<int(*)(int)>() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

  return 0;
}
