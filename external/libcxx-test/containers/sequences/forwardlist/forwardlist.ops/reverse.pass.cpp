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

// void reverse();

#include <forward_list>
#include <iterator>
#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
void test(int N)
{
    C c;
    for (int i = 0; i < N; ++i)
        c.push_front(i);
    c.reverse();
    TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == N);
    typename C::const_iterator j = c.begin();
    for (int i = 0; i < N; ++i, ++j)
        TC_ASSERT_EXPR(*j == i);
}

int tc_containers_sequences_forwardlist_forwardlist_ops_reverse(void) {
    for (int i = 0; i < 10; ++i)
        test<std::forward_list<int> >(i);
#if TEST_STD_VER >= 11
    for (int i = 0; i < 10; ++i)
        test<std::forward_list<int, min_allocator<int>> >(i);
#endif

  return 0;
}
