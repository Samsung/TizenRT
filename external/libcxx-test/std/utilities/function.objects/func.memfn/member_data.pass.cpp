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

// template<Returnable R, class T> unspecified mem_fn(R T::* pm);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    double data_;
};

template <class F>
TEST_CONSTEXPR_CXX20 bool
test(F f)
{
    {
    A a = {0.0};
    f(a) = 5;
    TC_ASSERT_EXPR(a.data_ == 5);
    A* ap = &a;
    f(ap) = 6;
    TC_ASSERT_EXPR(a.data_ == 6);
    const A* cap = ap;
    TC_ASSERT_EXPR(f(cap) == f(ap));
    const F& cf = f;
    TC_ASSERT_EXPR(cf(ap) == f(ap));
    }
    return true;
}

int tc_utilities_function_objects_func_memfn_member_data(void) {
    test(std::mem_fn(&A::data_));

#if TEST_STD_VER >= 20
    static_assert(test(std::mem_fn(&A::data_)));
#endif

    return 0;
}
