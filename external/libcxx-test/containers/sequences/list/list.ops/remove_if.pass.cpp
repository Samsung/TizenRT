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

// template <class Pred> void      remove_if(Pred pred); // before C++20
// template <class Pred> size_type remove_if(Pred pred); // c++20 and later

#include <list>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "min_allocator.h"
#include "counting_predicates.h"
#include "libcxx_tc_common.h"

bool even(int i)
{
    return i % 2 == 0;
}

bool g(int i)
{
    return i < 3;
}

struct PredLWG526 {
    PredLWG526 (int i) : i_(i) {};
    ~PredLWG526() { i_ = -32767; }
    bool operator() (const PredLWG526 &p) const { return p.i_ == i_; }

    bool operator==(int i) const { return i == i_;}
    int i_;
};

typedef unary_counting_predicate<bool(*)(int), int> Predicate;

int tc_containers_sequences_list_list_ops_remove_if(void) {
    {
    int a1[] = {1, 2, 3, 4};
    int a2[] = {3, 4};
    typedef std::list<int> L;
    L c(a1, a1+4);
    Predicate cp(g);
#if TEST_STD_VER > 17
    ASSERT_SAME_TYPE(L::size_type, decltype(c.remove_if(std::ref(cp))));
    TC_ASSERT_EXPR(c.remove_if(std::ref(cp)) == 2);
#else
    ASSERT_SAME_TYPE(void, decltype(c.remove_if(std::ref(cp))));
    c.remove_if(std::ref(cp));
#endif
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2+2));
    TC_ASSERT_EXPR(cp.count() == 4);
    }
    {
    int a1[] = {1, 2, 3, 4};
    int a2[] = {1, 3};
    std::list<int> c(a1, a1+4);
    Predicate cp(even);
#if TEST_STD_VER > 17
    TC_ASSERT_EXPR(c.remove_if(std::ref(cp)) == 2);
#else
    c.remove_if(std::ref(cp));
#endif
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2+2));
    TC_ASSERT_EXPR(cp.count() == 4);
    }
    { // LWG issue #526
    int a1[] = {1, 2, 1, 3, 5, 8, 11};
    int a2[] = {2, 3, 5, 8, 11};
    std::list<PredLWG526> c(a1, a1 + 7);
    c.remove_if(std::ref(c.front()));
    TC_ASSERT_EXPR(c.size() == 5);
    for (std::size_t i = 0; i < c.size(); ++i)
    {
        TC_ASSERT_EXPR(c.front() == a2[i]);
        c.pop_front();
    }
    }

#if TEST_STD_VER >= 11
    {
    int a1[] = {1, 2, 3, 4};
    int a2[] = {3, 4};
    std::list<int, min_allocator<int>> c(a1, a1+4);
    Predicate cp(g);
#if TEST_STD_VER > 17
    TC_ASSERT_EXPR(c.remove_if(std::ref(cp)) == 2);
#else
    c.remove_if(std::ref(cp));
#endif
    TC_ASSERT_EXPR((c == std::list<int, min_allocator<int>>(a2, a2+2)));
    TC_ASSERT_EXPR(cp.count() == 4);
    }
#endif

  return 0;
}
