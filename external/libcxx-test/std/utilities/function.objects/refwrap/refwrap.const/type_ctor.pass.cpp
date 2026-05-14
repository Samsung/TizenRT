//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <functional>

// reference_wrapper

// reference_wrapper(T& t);

#include <functional>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class functor1
{
};

template <class T>
void
test(T& t)
{
    std::reference_wrapper<T> r(t);
    TC_ASSERT_EXPR(&r.get() == &t);
}

void f() {}

int tc_utilities_function_objects_refwrap_refwrap_const_type_ctor(void) {
    void (*fp)() = f;
    test(fp);
    test(f);
    functor1 f1;
    test(f1);
    int i = 0;
    test(i);
    const int j = 0;
    test(j);

    {
    using Ref = std::reference_wrapper<int>;
    static_assert((std::is_constructible<Ref, int&>::value), "");
    static_assert((!std::is_constructible<Ref, int>::value), "");
    static_assert((!std::is_constructible<Ref, int&&>::value), "");
    }

#if TEST_STD_VER >= 11
    {
    using Ref = std::reference_wrapper<int>;
    static_assert((std::is_nothrow_constructible<Ref, int&>::value), "");
    static_assert((!std::is_nothrow_constructible<Ref, int>::value), "");
    static_assert((!std::is_nothrow_constructible<Ref, int&&>::value), "");
    }
#endif

  return 0;
}
