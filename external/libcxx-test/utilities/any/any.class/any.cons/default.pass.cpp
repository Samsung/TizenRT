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

// <any>

// any() noexcept;

#include <any>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "any_helpers.h"
#include "count_new.h"
#include "libcxx_tc_common.h"

int tc_utilities_any_any_class_any_cons_default(void) {
    {
        static_assert(
            std::is_nothrow_default_constructible<std::any>::value
          , "Must be default constructible"
          );
    }
    {
        struct TestConstexpr : public std::any {
          constexpr TestConstexpr() : std::any() {}
        };
        static TEST_CONSTINIT std::any a;
        (void)a;
    }
    {
        DisableAllocationGuard g; ((void)g);
        const std::any a;
        assertEmpty(a);
    }

  return 0;
}
