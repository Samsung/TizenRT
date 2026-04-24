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
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: no-rtti

// <any>

// any::type() noexcept

#include <any>
#include <cassert>

#include "test_macros.h"
#include "any_helpers.h"
#include "libcxx_tc_common.h"

int tc_utilities_any_any_class_any_observers_type(void) {
    {
        const std::any a;
        TC_ASSERT_EXPR(a.type() == typeid(void));
        ASSERT_NOEXCEPT(a.type());
    }
    {
        std::any a = small(1);
        TC_ASSERT_EXPR(a.type() == typeid(small));
    }
    {
        std::any a = large(1);
        TC_ASSERT_EXPR(a.type() == typeid(large));
    }
    {
        int arr[3];
        std::any a = arr;
        TC_ASSERT_EXPR(a.type() == typeid(int*));  // ensure that it is decayed
    }

  return 0;
}
