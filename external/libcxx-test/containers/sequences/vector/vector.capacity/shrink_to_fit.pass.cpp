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

// void shrink_to_fit();

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests() {
    {
        std::vector<int> v(100);
        v.push_back(1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v.shrink_to_fit();
        TC_ASSERT_EXPR(v.capacity() == 101);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int, limited_allocator<int, 401> > v(100);
        v.push_back(1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v.shrink_to_fit();
        TC_ASSERT_EXPR(v.capacity() == 101);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    if (!TEST_IS_CONSTANT_EVALUATED) {
        std::vector<int, limited_allocator<int, 400> > v(100);
        v.push_back(1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v.shrink_to_fit();
        LIBCPP_ASSERT(v.capacity() == 200); // assumes libc++'s 2x growth factor
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#endif
#if TEST_STD_VER >= 11
    {
        std::vector<int, min_allocator<int>> v(100);
        v.push_back(1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v.shrink_to_fit();
        TC_ASSERT_EXPR(v.capacity() == 101);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
      std::vector<int, safe_allocator<int>> v(100);
      v.push_back(1);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
      v.shrink_to_fit();
      TC_ASSERT_EXPR(v.capacity() == 101);
      TC_ASSERT_EXPR(v.size() == 101);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_capacity_shrink_to_fit(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
