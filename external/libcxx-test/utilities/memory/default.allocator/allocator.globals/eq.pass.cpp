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

// allocator:

// template <class T1, class T2>
//   constexpr bool
//   operator==(const allocator<T1>&, const allocator<T2>&) throw();
//
// template <class T1, class T2>
//   constexpr bool
//   operator!=(const allocator<T1>&, const allocator<T2>&) throw();

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test()
{
    std::allocator<int> a1;
    std::allocator<int> a2;
    TC_ASSERT_EXPR(a1 == a2);
    TC_ASSERT_EXPR(!(a1 != a2));

    return true;
}

int tc_utilities_memory_default_allocator_allocator_globals_eq(void) {
    test();

#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return 0;
}
