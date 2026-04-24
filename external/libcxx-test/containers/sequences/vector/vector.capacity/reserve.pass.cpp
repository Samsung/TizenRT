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

// void reserve(size_type n);

#include <vector>
#include <cassert>
#include <stdexcept>
#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests() {
    {
        std::vector<int> v;
        v.reserve(10);
        TC_ASSERT_EXPR(v.capacity() >= 10);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int> v(100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(50);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(150);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 150);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        // Add 1 for implementations that dynamically allocate a container proxy.
        std::vector<int, limited_allocator<int, 250 + 1> > v(100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(50);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(150);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 150);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    if (!TEST_IS_CONSTANT_EVALUATED) {
        std::vector<int> v;
        std::size_t sz = v.max_size() + 1;

#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            v.reserve(sz);
            TC_ASSERT_EXPR(false);
        } catch (const std::length_error&) {
            TC_ASSERT_EXPR(v.size() == 0);
            TC_ASSERT_EXPR(v.capacity() == 0);
        }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
    if (!TEST_IS_CONSTANT_EVALUATED) {
        std::vector<int> v(10, 42);
        int* previous_data = v.data();
        std::size_t previous_capacity = v.capacity();
        std::size_t sz = v.max_size() + 1;

#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            v.reserve(sz);
            TC_ASSERT_EXPR(false);
        } catch (std::length_error&) {
            TC_ASSERT_EXPR(v.size() == 10);
            TC_ASSERT_EXPR(v.capacity() == previous_capacity);
            TC_ASSERT_EXPR(v.data() == previous_data);

            for (int i = 0; i < 10; ++i) {
                TC_ASSERT_EXPR(v[i] == 42);
            }
#endif // _LIBCPP_NO_EXCEPTIONS
        }
    }
#endif
#if TEST_STD_VER >= 11
    {
        std::vector<int, min_allocator<int>> v;
        v.reserve(10);
        TC_ASSERT_EXPR(v.capacity() >= 10);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int, min_allocator<int>> v(100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(50);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(150);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 150);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
      std::vector<int, safe_allocator<int>> v;
      v.reserve(10);
      TC_ASSERT_EXPR(v.capacity() >= 10);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
      std::vector<int, safe_allocator<int>> v(100);
      TC_ASSERT_EXPR(v.capacity() == 100);
      v.reserve(50);
      TC_ASSERT_EXPR(v.size() == 100);
      TC_ASSERT_EXPR(v.capacity() == 100);
      v.reserve(150);
      TC_ASSERT_EXPR(v.size() == 100);
      TC_ASSERT_EXPR(v.capacity() == 150);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#endif
#ifndef TEST_HAS_NO_EXCEPTIONS
    if (!TEST_IS_CONSTANT_EVALUATED) {
        std::vector<int, limited_allocator<int, 100> > v;
        v.reserve(50);
        TC_ASSERT_EXPR(v.capacity() == 50);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
#ifndef _LIBCPP_NO_EXCEPTIONS
        try {
            v.reserve(101);
            TC_ASSERT_EXPR(false);
        } catch (const std::length_error&) {
            // no-op
        }
#endif // _LIBCPP_NO_EXCEPTIONS
        TC_ASSERT_EXPR(v.capacity() == 50);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_capacity_reserve(void) {
  tests();

#if TEST_STD_VER > 17
  static_assert(tests());
#endif

  return 0;
}
