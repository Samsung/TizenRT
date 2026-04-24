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
