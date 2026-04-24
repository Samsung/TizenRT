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
// <vector>
// vector<bool>

// static void swap(reference x, reference y) noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{

    bool a[] = {false, true, false, true};
    bool* an = a + sizeof(a)/sizeof(a[0]);

    std::vector<bool> v(a, an);
    std::vector<bool>::reference r1 = v[0];
    std::vector<bool>::reference r2 = v[3];

#if TEST_STD_VER >= 11
    static_assert((noexcept(v.swap(r1,r2))), "");
#endif

    TC_ASSERT_EXPR(!r1);
    TC_ASSERT_EXPR( r2);
    v.swap(r1, r2);
    TC_ASSERT_EXPR( r1);
    TC_ASSERT_EXPR(!r2);

    return true;
}

int tc_containers_sequences_vector_bool_reference_swap(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
