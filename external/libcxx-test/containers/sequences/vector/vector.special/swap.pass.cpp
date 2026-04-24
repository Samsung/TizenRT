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

// template <class T, class Alloc>
//   void swap(vector<T,Alloc>& x, vector<T,Alloc>& y);

#include <vector>
#include <iterator>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "asan_testing.h"
#include "libcxx_tc_common.h"

template <typename A>
TEST_CONSTEXPR_CXX20 void test_with_allocator() {
  {
    int a1[] = {1, 3, 7, 9, 10};
    int a2[] = {0, 2, 4, 5, 6, 8, 11};
    std::vector<int, A> c1(a1, a1 + sizeof(a1) / sizeof(a1[0]));
    std::vector<int, A> c2(a2, a2 + sizeof(a2) / sizeof(a2[0]));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    swap(c1, c2);
    TC_ASSERT_EXPR((c1 == std::vector<int, A>(a2, a2 + sizeof(a2) / sizeof(a2[0]))));
    TC_ASSERT_EXPR((c2 == std::vector<int, A>(a1, a1 + sizeof(a1) / sizeof(a1[0]))));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
  }
  {
    int a1[] = {1, 3, 7, 9, 10};
    int a2[] = {0, 2, 4, 5, 6, 8, 11};
    std::vector<int, A> c1(a1, a1);
    std::vector<int, A> c2(a2, a2 + sizeof(a2) / sizeof(a2[0]));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    swap(c1, c2);
    TC_ASSERT_EXPR((c1 == std::vector<int, A>(a2, a2 + sizeof(a2) / sizeof(a2[0]))));
    TC_ASSERT_EXPR(c2.empty());
    TC_ASSERT_EXPR(std::distance(c2.begin(), c2.end()) == 0);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
  }
  {
    int a1[] = {1, 3, 7, 9, 10};
    int a2[] = {0, 2, 4, 5, 6, 8, 11};
    std::vector<int, A> c1(a1, a1 + sizeof(a1) / sizeof(a1[0]));
    std::vector<int, A> c2(a2, a2);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    swap(c1, c2);
    TC_ASSERT_EXPR(c1.empty());
    TC_ASSERT_EXPR(std::distance(c1.begin(), c1.end()) == 0);
    TC_ASSERT_EXPR((c2 == std::vector<int, A>(a1, a1 + sizeof(a1) / sizeof(a1[0]))));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
  }
  {
    int a1[] = {1, 3, 7, 9, 10};
    int a2[] = {0, 2, 4, 5, 6, 8, 11};
    std::vector<int, A> c1(a1, a1);
    std::vector<int, A> c2(a2, a2);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    swap(c1, c2);
    TC_ASSERT_EXPR(c1.empty());
    TC_ASSERT_EXPR(std::distance(c1.begin(), c1.end()) == 0);
    TC_ASSERT_EXPR(c2.empty());
    TC_ASSERT_EXPR(std::distance(c2.begin(), c2.end()) == 0);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
  }
  {
    int a1[] = {1, 3, 7, 9, 10};
    int a2[] = {0, 2, 4, 5, 6, 8, 11};
    std::vector<int, A> c1(a1, a1 + sizeof(a1) / sizeof(a1[0]), A());
    std::vector<int, A> c2(a2, a2 + sizeof(a2) / sizeof(a2[0]), A());
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    swap(c1, c2);
    TC_ASSERT_EXPR((c1 == std::vector<int, A>(a2, a2 + sizeof(a2) / sizeof(a2[0]))));
    TC_ASSERT_EXPR(c1.get_allocator() == A());
    TC_ASSERT_EXPR((c2 == std::vector<int, A>(a1, a1 + sizeof(a1) / sizeof(a1[0]))));
    TC_ASSERT_EXPR(c2.get_allocator() == A());
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
  }
}

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        std::vector<int> c2(a2, a2+sizeof(a2)/sizeof(a2[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
        swap(c1, c2);
        TC_ASSERT_EXPR(c1 == std::vector<int>(a2, a2+sizeof(a2)/sizeof(a2[0])));
        TC_ASSERT_EXPR(c2 == std::vector<int>(a1, a1+sizeof(a1)/sizeof(a1[0])));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int> c1(a1, a1);
        std::vector<int> c2(a2, a2+sizeof(a2)/sizeof(a2[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
        swap(c1, c2);
        TC_ASSERT_EXPR(c1 == std::vector<int>(a2, a2+sizeof(a2)/sizeof(a2[0])));
        TC_ASSERT_EXPR(c2.empty());
        TC_ASSERT_EXPR(std::distance(c2.begin(), c2.end()) == 0);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        std::vector<int> c2(a2, a2);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
        swap(c1, c2);
        TC_ASSERT_EXPR(c1.empty());
        TC_ASSERT_EXPR(std::distance(c1.begin(), c1.end()) == 0);
        TC_ASSERT_EXPR(c2 == std::vector<int>(a1, a1+sizeof(a1)/sizeof(a1[0])));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::vector<int> c1(a1, a1);
        std::vector<int> c2(a2, a2);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
        swap(c1, c2);
        TC_ASSERT_EXPR(c1.empty());
        TC_ASSERT_EXPR(std::distance(c1.begin(), c1.end()) == 0);
        TC_ASSERT_EXPR(c2.empty());
        TC_ASSERT_EXPR(std::distance(c2.begin(), c2.end()) == 0);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        typedef test_allocator<int> A;
        std::vector<int, A> c1(a1, a1+sizeof(a1)/sizeof(a1[0]), A(1, 1));
        std::vector<int, A> c2(a2, a2+sizeof(a2)/sizeof(a2[0]), A(1, 2));
        swap(c1, c2);
        TC_ASSERT_EXPR((c1 == std::vector<int, A>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
        TC_ASSERT_EXPR(c1.get_allocator().get_id() == 1);
        TC_ASSERT_EXPR((c2 == std::vector<int, A>(a1, a1+sizeof(a1)/sizeof(a1[0]))));
        TC_ASSERT_EXPR(c2.get_allocator().get_id() == 2);
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        typedef other_allocator<int> A;
        std::vector<int, A> c1(a1, a1+sizeof(a1)/sizeof(a1[0]), A(1));
        std::vector<int, A> c2(a2, a2+sizeof(a2)/sizeof(a2[0]), A(2));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
        swap(c1, c2);
        TC_ASSERT_EXPR((c1 == std::vector<int, A>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
        TC_ASSERT_EXPR(c1.get_allocator() == A(2));
        TC_ASSERT_EXPR((c2 == std::vector<int, A>(a1, a1+sizeof(a1)/sizeof(a1[0]))));
        TC_ASSERT_EXPR(c2.get_allocator() == A(1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    }
#if TEST_STD_VER >= 11
    test_with_allocator<min_allocator<int>>();
    test_with_allocator<safe_allocator<int>>();
#endif

    return true;
}

int tc_containers_sequences_vector_vector_special_swap(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
