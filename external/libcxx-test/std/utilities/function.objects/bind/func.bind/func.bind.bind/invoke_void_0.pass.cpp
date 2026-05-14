//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <functional>

// template<CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);
// template<Returnable R, CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int count = 0;

template <class F>
void
test(F f)
{
    int save_count = count;
    f();
    TC_ASSERT_EXPR(count == save_count + 1);
}

template <class F>
void
test_const(const F& f)
{
    int save_count = count;
    f();
    TC_ASSERT_EXPR(count == save_count + 2);
}

void f() {++count;}

int g() {++count; return 0;}

struct A_void_0
{
    void operator()() {++count;}
    void operator()() const {count += 2;}
};

struct A_int_0
{
    int operator()() {++count; return 4;}
    int operator()() const {count += 2; return 5;}
};

int tc_utilities_function_objects_bind_func_bind_func_bind_bind_invoke_void_0(void) {
    test(std::bind(f));
    test(std::bind(&f));
    test(std::bind(A_void_0()));
    test_const(std::bind(A_void_0()));

    test(std::bind<void>(f));
    test(std::bind<void>(&f));
    test(std::bind<void>(A_void_0()));
    test_const(std::bind<void>(A_void_0()));

    test(std::bind<void>(g));
    test(std::bind<void>(&g));
    test(std::bind<void>(A_int_0()));
    test_const(std::bind<void>(A_int_0()));

  return 0;
}
