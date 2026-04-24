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

// void swap(any &, any &) noexcept

// swap(...) just wraps any::swap(...). That function is tested elsewhere.

#include <any>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_any_any_nonmembers_swap(void) {

    { // test noexcept
        std::any a;
        static_assert(noexcept(swap(a, a)), "swap(any&, any&) must be noexcept");
    }
    {
        std::any a1 = 1;
        std::any a2 = 2;

        swap(a1, a2);

        TC_ASSERT_EXPR(std::any_cast<int>(a1) == 2);
        TC_ASSERT_EXPR(std::any_cast<int>(a2) == 1);
    }

  return 0;
}
