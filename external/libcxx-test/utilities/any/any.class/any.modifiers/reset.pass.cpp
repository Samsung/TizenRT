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

// XFAIL: availability-bad_any_cast-missing && !no-exceptions

// <any>

// any::reset() noexcept

#include <any>
#include <cassert>

#include "test_macros.h"
#include "any_helpers.h"
#include "libcxx_tc_common.h"

int tc_utilities_any_any_class_any_modifiers_reset(void) {
    // empty
    {
        std::any a;
        ASSERT_NOEXCEPT(a.reset());

        assertEmpty(a);

        a.reset();

        assertEmpty(a);
    }
    // small object
    {
        std::any a = small(1);
        TC_ASSERT_EXPR(small::count == 1);
        assertContains<small>(a, 1);

        a.reset();

        assertEmpty<small>(a);
        TC_ASSERT_EXPR(small::count == 0);
    }
    // large object
    {
        std::any a = large(1);
        TC_ASSERT_EXPR(large::count == 1);
        assertContains<large>(a, 1);

        a.reset();

        assertEmpty<large>(a);
        TC_ASSERT_EXPR(large::count == 0);
    }

  return 0;
}
