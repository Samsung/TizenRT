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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <memory>

// shared_ptr

// template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r);

#include <memory>
#include <type_traits>
#include <cassert>
#include "libcxx_tc_common.h"
#include "smartptr_shared_common.hpp"

int tc_libcxx_utilities_util_smartptr_shared_assign_shared_ptr_Y(void)
{
    A::count = 0;
    B::count = 0;
    {
        const std::shared_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = pA;
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(pB.use_count() == 2);
            TC_ASSERT_EXPR(pA.use_count() == 2);
            TC_ASSERT_EXPR(pA.get() == pB.get());
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        const std::shared_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = pA;
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pB.use_count() == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
            TC_ASSERT_EXPR(pA.get() == pB.get());
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        const std::shared_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = pA;
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(pB.use_count() == 2);
            TC_ASSERT_EXPR(pA.use_count() == 2);
            TC_ASSERT_EXPR(pA.get() == pB.get());
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        const std::shared_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = pA;
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pB.use_count() == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
            TC_ASSERT_EXPR(pA.get() == pB.get());
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    TC_SUCCESS_RESULT();
    return 0;
}
