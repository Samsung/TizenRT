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

// template<Returnable R, class T, CopyConstructible... Args>
//   unspecified mem_fn(R (T::* pm)(Args...));

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    TEST_CONSTEXPR_CXX14 char test0() {return 'a';}
    TEST_CONSTEXPR_CXX14 char test1(int) {return 'b';}
    TEST_CONSTEXPR_CXX14 char test2(int, double) {return 'c';}
};

template <class F>
TEST_CONSTEXPR_CXX20 bool
test0(F f)
{
    {
    A a;
    TC_ASSERT_EXPR(f(a) == 'a');
    A* ap = &a;
    TC_ASSERT_EXPR(f(ap) == 'a');
    const F& cf = f;
    TC_ASSERT_EXPR(cf(ap) == 'a');
    }
    return true;
}

template <class F>
TEST_CONSTEXPR_CXX20 bool
test1(F f)
{
    {
    A a;
    TC_ASSERT_EXPR(f(a, 1) == 'b');
    A* ap = &a;
    TC_ASSERT_EXPR(f(ap, 2) == 'b');
    const F& cf = f;
    TC_ASSERT_EXPR(cf(ap, 2) == 'b');
    }
    return true;
}

template <class F>
TEST_CONSTEXPR_CXX20 bool
test2(F f)
{
    {
    A a;
    TC_ASSERT_EXPR(f(a, 1, 2) == 'c');
    A* ap = &a;
    TC_ASSERT_EXPR(f(ap, 2, 3.5) == 'c');
    const F& cf = f;
    TC_ASSERT_EXPR(cf(ap, 2, 3.5) == 'c');
    }
    return true;
}

int tc_utilities_function_objects_func_memfn_member_function(void) {
    test0(std::mem_fn(&A::test0));
    test1(std::mem_fn(&A::test1));
    test2(std::mem_fn(&A::test2));
#if TEST_STD_VER >= 11
    static_assert((noexcept(std::mem_fn(&A::test0))), ""); // LWG#2489
#endif

#if TEST_STD_VER >= 20
    static_assert(test0(std::mem_fn(&A::test0)));
    static_assert(test1(std::mem_fn(&A::test1)));
    static_assert(test2(std::mem_fn(&A::test2)));
#endif

    return 0;
}
