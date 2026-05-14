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
// type_traits

// is_swappable

// IMPORTANT: The include order is part of the test. We want to pick up
// the following definitions in this order:
//   1) is_swappable, is_nothrow_swappable
//   2) iter_swap, swap_ranges
//   3) swap(T (&)[N], T (&)[N])
// This test checks that (1) and (2) see forward declarations
// for (3).
#include <type_traits>
#include <algorithm>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_meta_meta_unary_meta_unary_prop_is_swappable_include_order(void) {
    // Use a builtin type so we don't get ADL lookup.
    typedef double T[17][29];
    {
        LIBCPP_STATIC_ASSERT(std::__is_swappable<T>::value, "");
#if TEST_STD_VER > 14
        static_assert(std::is_swappable_v<T>, "");
#endif
    }
    {
        T t1 = {};
        T t2 = {};
       std::iter_swap(t1, t2);
       std::swap_ranges(t1, t1 + 17, t2);
    }

  return 0;
}
