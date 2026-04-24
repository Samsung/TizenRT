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
// <forward_list>

// template <class T, class Allocator>
//     bool operator==(const forward_list<T, Allocator>& x,
//                     const forward_list<T, Allocator>& y);
//
// template <class T, class Allocator>
//     bool operator!=(const forward_list<T, Allocator>& x,
//                     const forward_list<T, Allocator>& y);

#include <forward_list>
#include <iterator>
#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
void test(int N, int M)
{
    C c1;
    for (int i = 0; i < N; ++i)
        c1.push_front(i);
    C c2;
    for (int i = 0; i < M; ++i)
        c2.push_front(i);
    if (N == M)
        TC_ASSERT_EXPR(c1 == c2);
    else
        TC_ASSERT_EXPR(c1 != c2);
    c2 = c1;
    TC_ASSERT_EXPR(c1 == c2);
    if (N > 0)
    {
        c2.front() = N+1;
        TC_ASSERT_EXPR(c1 != c2);
    }
}

int tc_containers_sequences_forwardlist_forwardlist_spec_equal(void) {
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            test<std::forward_list<int> >(i, j);
#if TEST_STD_VER >= 11
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            test<std::forward_list<int, min_allocator<int>> >(i, j);
#endif

  return 0;
}
