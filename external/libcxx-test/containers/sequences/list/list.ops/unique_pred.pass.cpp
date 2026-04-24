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

// template <class BinaryPred> void      unique(BinaryPred pred); // before C++20
// template <class BinaryPred> size_type unique(BinaryPred pred); // C++20 and later

#include <list>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

bool g(int x, int y)
{
    return x == y;
}

struct PredLWG526 {
    PredLWG526 (int i) : i_(i) {};
    ~PredLWG526() { i_ = -32767; }
    bool operator() (const PredLWG526 &lhs, const PredLWG526 &rhs) const { return lhs.i_ == rhs.i_; }

    bool operator==(int i) const { return i == i_;}
    int i_;
};

int tc_containers_sequences_list_list_ops_unique_pred(void) {
    {
    int a1[] = {2, 1, 1, 4, 4, 4, 4, 3, 3};
    int a2[] = {2, 1, 4, 3};
    typedef std::list<int> L;
    L c(a1, a1+sizeof(a1)/sizeof(a1[0]));
#if TEST_STD_VER > 17
    ASSERT_SAME_TYPE(L::size_type, decltype(c.unique(g)));
    TC_ASSERT_EXPR(c.unique(g) == 5);
#else
    ASSERT_SAME_TYPE(void,         decltype(c.unique(g)));
    c.unique(g);
#endif
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2+4));
    }

    { // LWG issue #526
    int a1[] = {1, 1, 1, 2, 3, 5, 5, 2, 11};
    int a2[] = {1,       2, 3, 5,    2, 11};
    std::list<PredLWG526> c(a1, a1 + 9);
#if TEST_STD_VER > 17
    TC_ASSERT_EXPR(c.unique(std::ref(c.front())) == 3);
#else
    c.unique(std::ref(c.front()));
#endif
    TC_ASSERT_EXPR(c.size() == 6);
    for (std::size_t i = 0; i < c.size(); ++i)
    {
        TC_ASSERT_EXPR(c.front() == a2[i]);
        c.pop_front();
    }
    }

#if TEST_STD_VER >= 11
    {
    int a1[] = {2, 1, 1, 4, 4, 4, 4, 3, 3};
    int a2[] = {2, 1, 4, 3};
    std::list<int, min_allocator<int>> c(a1, a1+sizeof(a1)/sizeof(a1[0]));
#if TEST_STD_VER > 17
    TC_ASSERT_EXPR(c.unique(g) == 5);
#else
    c.unique(g);
#endif
    TC_ASSERT_EXPR((c == std::list<int, min_allocator<int>>(a2, a2+4)));
    }
#endif

  return 0;
}
