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
// <list>

// void sort();

#include <algorithm>
#include <list>
#include <random>
#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

std::mt19937 randomness;

struct Payload
{
    int val;
    int side;
    Payload(int v) : val(v), side(0) {}
    Payload(int v, int s) : val(v), side(s) {}
    bool operator< (const Payload &rhs) const { return val <  rhs.val;}
//     bool operator==(const Payload &rhs) const { return val == rhs.val;}
};

void test_stable(int N)
{
    typedef Payload T;
    typedef std::list<T> C;
    typedef std::vector<T> V;
    V v;
    for (int i = 0; i < N; ++i)
        v.push_back(Payload(i/2));
    std::shuffle(v.begin(), v.end(), randomness);
    for (int i = 0; i < N; ++i)
        v[i].side = i;

    C c(v.begin(), v.end());
    c.sort();
    TC_ASSERT_EXPR(std::distance(c.begin(), c.end()) == N);

//  Are we sorted?
    typename C::const_iterator j = c.begin();
    for (int i = 0; i < N; ++i, ++j)
        TC_ASSERT_EXPR(j->val == i/2);

//  Are we stable?
    for (C::const_iterator it = c.begin(); it != c.end(); ++it)
    {
        C::const_iterator next = std::next(it);
        if (next != c.end() && it->val == next->val)
            TC_ASSERT_EXPR(it->side < next->side);
    }
}


int tc_containers_sequences_list_list_ops_sort(void) {
    {
    int a1[] = {4, 8, 1, 0, 5, 7, 2, 3, 6, 11, 10, 9};
    int a2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::list<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
    c1.sort();
    TC_ASSERT_EXPR(c1 == std::list<int>(a2, a2+sizeof(a2)/sizeof(a2[0])));
    }
#if TEST_STD_VER >= 11
    {
    int a1[] = {4, 8, 1, 0, 5, 7, 2, 3, 6, 11, 10, 9};
    int a2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::list<int, min_allocator<int>> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
    c1.sort();
    TC_ASSERT_EXPR((c1 == std::list<int, min_allocator<int>>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
    }
#endif

    for (int i = 0; i < 40; ++i)
        test_stable(i);

  return 0;
}
