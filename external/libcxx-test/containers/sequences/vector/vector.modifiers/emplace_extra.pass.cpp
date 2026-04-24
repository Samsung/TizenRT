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

// template <class... Args> iterator emplace(const_iterator pos, Args&&... args);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests() {
    {
        std::vector<int> v;
        v.reserve(3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v = { 1, 2, 3 };
        v.emplace(v.begin(), v.back());
        TC_ASSERT_EXPR(v[0] == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int> v;
        v.reserve(4);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v = { 1, 2, 3 };
        v.emplace(v.begin(), v.back());
        TC_ASSERT_EXPR(v[0] == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int, min_allocator<int>> v;
        v.reserve(3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v = { 1, 2, 3 };
        v.emplace(v.begin(), v.back());
        TC_ASSERT_EXPR(v[0] == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int, min_allocator<int>> v;
        v.reserve(4);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v = { 1, 2, 3 };
        v.emplace(v.begin(), v.back());
        TC_ASSERT_EXPR(v[0] == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
      std::vector<int, safe_allocator<int>> v;
      v.reserve(3);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
      v = {1, 2, 3};
      v.emplace(v.begin(), v.back());
      TC_ASSERT_EXPR(v[0] == 3);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
      std::vector<int, safe_allocator<int>> v;
      v.reserve(4);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
      v = {1, 2, 3};
      v.emplace(v.begin(), v.back());
      TC_ASSERT_EXPR(v[0] == 3);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int> v;
        v.reserve(8);
        std::size_t old_capacity = v.capacity();
        TC_ASSERT_EXPR(old_capacity >= 8);

        v.resize(4); // keep the existing capacity
        TC_ASSERT_EXPR(v.capacity() == old_capacity);

        v.emplace(v.cend(), 42);
        TC_ASSERT_EXPR(v.size() == 5);
        TC_ASSERT_EXPR(v.capacity() == old_capacity);
        TC_ASSERT_EXPR(v[4] == 42);
    }

    return true;
}

int tc_containers_sequences_vector_vector_modifiers_emplace_extra(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
