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
// <memory>

// shared_ptr

// template<class D, class T> D* get_deleter(const shared_ptr<T>& p);

// UNSUPPORTED: no-rtti

#include <memory>
#include <cassert>
#include "test_macros.h"
#include "deleter_types.h"
#include "libcxx_tc_common.h"

struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_getdeleter_get_deleter(void) {
    {
        {
            A* ptr = new A;
            std::shared_ptr<A> p(ptr, test_deleter<A>(3));
            test_deleter<A>* d = std::get_deleter<test_deleter<A> >(p);
            TC_ASSERT_EXPR(test_deleter<A>::count == 1);
            TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 0);
            TC_ASSERT_EXPR(d);
            TC_ASSERT_EXPR(d->state() == 3);
        }
        TC_ASSERT_EXPR(A::count == 0);
        TC_ASSERT_EXPR(test_deleter<A>::count == 0);
        TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 1);
    }
    test_deleter<A>::dealloc_count = 0;
    {
        {
            std::shared_ptr<A> p(nullptr, test_deleter<A>(3));
            test_deleter<A>* d = std::get_deleter<test_deleter<A> >(p);
            TC_ASSERT_EXPR(test_deleter<A>::count == 1);
            TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 0);
            TC_ASSERT_EXPR(d);
            TC_ASSERT_EXPR(d->state() == 3);
        }
        TC_ASSERT_EXPR(A::count == 0);
        TC_ASSERT_EXPR(test_deleter<A>::count == 0);
        TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 1);
    }
    test_deleter<A>::dealloc_count = 0;
    {
        std::shared_ptr<A> p(nullptr, test_deleter<A>(3));
        std::default_delete<A>* d = std::get_deleter<std::default_delete<A> >(p);
        TC_ASSERT_EXPR(d == 0);
    }

  return 0;
}
