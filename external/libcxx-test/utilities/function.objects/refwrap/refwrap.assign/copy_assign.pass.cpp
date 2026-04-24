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

// reference_wrapper& operator=(const reference_wrapper<T>& x);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class functor1
{
};

struct convertible_to_int_ref {
    int val = 0;
    operator int&() { return val; }
    operator int const&() const { return val; }
};

template <class T>
void
test(T& t)
{
    std::reference_wrapper<T> r(t);
    T t2 = t;
    std::reference_wrapper<T> r2(t2);
    r2 = r;
    TC_ASSERT_EXPR(&r2.get() == &t);
}

void f() {}
void g() {}

void
test_function()
{
    std::reference_wrapper<void ()> r(f);
    std::reference_wrapper<void ()> r2(g);
    r2 = r;
    TC_ASSERT_EXPR(&r2.get() == &f);
}

int tc_utilities_function_objects_refwrap_refwrap_assign_copy_assign(void) {
    void (*fp)() = f;
    test(fp);
    test_function();
    functor1 f1;
    test(f1);
    int i = 0;
    test(i);
    const int j = 0;
    test(j);

#if TEST_STD_VER >= 11
    convertible_to_int_ref convi;
    test(convi);
    convertible_to_int_ref const convic;
    test(convic);

    {
    using Ref = std::reference_wrapper<int>;
    static_assert((std::is_assignable<Ref&, int&>::value), "");
    static_assert((!std::is_assignable<Ref&, int>::value), "");
    static_assert((!std::is_assignable<Ref&, int&&>::value), "");
    }
#endif

  return 0;
}
