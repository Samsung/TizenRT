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
// UNSUPPORTED: c++03

// <vector>

// void assign(initializer_list<value_type> il);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

template <typename Vec>
TEST_CONSTEXPR_CXX20 void test(Vec &v)
{
    v.assign({3, 4, 5, 6});
    TC_ASSERT_EXPR(v.size() == 4);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    TC_ASSERT_EXPR(v[0] == 3);
    TC_ASSERT_EXPR(v[1] == 4);
    TC_ASSERT_EXPR(v[2] == 5);
    TC_ASSERT_EXPR(v[3] == 6);
}

TEST_CONSTEXPR_CXX20 bool tests() {
    {
    typedef std::vector<int> V;
    V d1;
    V d2;
    d2.reserve(10);  // no reallocation during assign.
    test(d1);
    test(d2);
    }
    {
    typedef std::vector<int, min_allocator<int>> V;
    V d1;
    V d2;
    d2.reserve(10);  // no reallocation during assign.
    test(d1);
    test(d2);
    }

    return true;
}

int tc_containers_sequences_vector_vector_cons_assign_initializer_list(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
