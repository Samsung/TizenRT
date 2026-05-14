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

// iterator insert(const_iterator position, const value_type& x);

#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool test() {

    {
        std::vector<int> v(100);
        const int lvalue = 1;
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, lvalue);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < 101; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        const std::size_t n = 100;
        std::vector<int> v(n);
        v.reserve(n + 1);
        const int lvalue = 1;

        // no reallocation expected
        std::vector<int>::iterator it = v.insert(v.cbegin() + n, lvalue);

        TC_ASSERT_EXPR(v.size() == n + 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(it == v.begin() + n);
        for (std::size_t i = 0; i < n; ++i) {
            TC_ASSERT_EXPR(v[i] == 0);
        }
        TC_ASSERT_EXPR(v[n] == lvalue);
    }
    {
        std::vector<int> v(100);
        while(v.size() < v.capacity()) v.push_back(0); // force reallocation
        std::size_t sz = v.size();
        const int lvalue = 1;
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, lvalue);
        TC_ASSERT_EXPR(v.size() == sz + 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<int> v(100);
        while(v.size() < v.capacity()) v.push_back(0);
        v.pop_back(); v.pop_back(); // force no reallocation
        std::size_t sz = v.size();
        const int lvalue = 1;
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, lvalue);
        TC_ASSERT_EXPR(v.size() == sz + 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<int, limited_allocator<int, 300> > v(100);
        const int lvalue = 1;
        std::vector<int, limited_allocator<int, 300> >::iterator i = v.insert(v.cbegin() + 10, lvalue);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < 101; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
#if TEST_STD_VER >= 11
    {
        std::vector<int, min_allocator<int>> v(100);
        const int lvalue = 1;
        std::vector<int, min_allocator<int>>::iterator i = v.insert(v.cbegin() + 10, lvalue);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < 101; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
      std::vector<int, safe_allocator<int>> v(100);
      const int lvalue                                  = 1;
      std::vector<int, safe_allocator<int>>::iterator i = v.insert(v.cbegin() + 10, lvalue);
      TC_ASSERT_EXPR(v.size() == 101);
      TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
      TC_ASSERT_EXPR(i == v.begin() + 10);
      int j;
      for (j = 0; j < 10; ++j)
        TC_ASSERT_EXPR(v[j] == 0);
      TC_ASSERT_EXPR(v[j] == 1);
      for (++j; j < 101; ++j)
        TC_ASSERT_EXPR(v[j] == 0);
    }
#endif

    return true;
}

int tc_containers_sequences_vector_vector_modifiers_insert_iter_lvalue(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif

  return 0;
}
