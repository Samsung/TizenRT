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
//   unspecified mem_fn(R (T::* pm)(Args...) const volatile);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    char test0() const volatile {return 'a';}
    char test1(int) const volatile {return 'b';}
    char test2(int, double) const volatile {return 'c';}
};

template <class F>
void
test0(F f)
{
    {
    A a;
    TC_ASSERT_EXPR(f(a) == 'a');
    A* ap = &a;
    TC_ASSERT_EXPR(f(ap) == 'a');
    const volatile A* cap = &a;
    TC_ASSERT_EXPR(f(cap) == 'a');
    const F& cf = f;
    TC_ASSERT_EXPR(cf(ap) == 'a');
    }
}

template <class F>
void
test1(F f)
{
    {
    A a;
    TC_ASSERT_EXPR(f(a, 1) == 'b');
    A* ap = &a;
    TC_ASSERT_EXPR(f(ap, 2) == 'b');
    const volatile A* cap = &a;
    TC_ASSERT_EXPR(f(cap, 2) == 'b');
    const F& cf = f;
    TC_ASSERT_EXPR(cf(ap, 2) == 'b');
    }
}

template <class F>
void
test2(F f)
{
    {
    A a;
    TC_ASSERT_EXPR(f(a, 1, 2) == 'c');
    A* ap = &a;
    TC_ASSERT_EXPR(f(ap, 2, 3.5) == 'c');
    const volatile A* cap = &a;
    TC_ASSERT_EXPR(f(cap, 2, 3.5) == 'c');
    const F& cf = f;
    TC_ASSERT_EXPR(cf(ap, 2, 3.5) == 'c');
    }
}

int tc_utilities_function_objects_func_memfn_member_function_const_volatile(void) {
    test0(std::mem_fn(&A::test0));
    test1(std::mem_fn(&A::test1));
    test2(std::mem_fn(&A::test2));

  return 0;
}
