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

// weak_ptr

// bool expired() const;

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_obs_expired(void) {
    {
        std::weak_ptr<A> wp;
        TC_ASSERT_EXPR(wp.use_count() == 0);
        TC_ASSERT_EXPR(wp.expired() == (wp.use_count() == 0));
    }
    {
        std::shared_ptr<A> sp0(new A);
        std::weak_ptr<A> wp(sp0);
        TC_ASSERT_EXPR(wp.use_count() == 1);
        TC_ASSERT_EXPR(wp.expired() == (wp.use_count() == 0));
        sp0.reset();
        TC_ASSERT_EXPR(wp.use_count() == 0);
        TC_ASSERT_EXPR(wp.expired() == (wp.use_count() == 0));
    }

  return 0;
}
