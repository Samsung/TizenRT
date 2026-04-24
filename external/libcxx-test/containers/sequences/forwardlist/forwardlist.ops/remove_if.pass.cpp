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

// template <class Predicate> void      remove_if(Predicate pred); // C++17 and before
// template <class Predicate> size_type remove_if(Predicate pred); // C++20 and after

#include <forward_list>
#include <iterator>
#include <cassert>
#include <cstddef>
#include <functional>

#include "test_macros.h"
#include "min_allocator.h"
#include "counting_predicates.h"
#include "libcxx_tc_common.h"


template <class L, class Predicate>
void do_remove_if(L &l, Predicate pred, typename L::size_type expected)
{
    typename L::size_type old_size = std::distance(l.begin(), l.end());
#if TEST_STD_VER > 17
    ASSERT_SAME_TYPE(decltype(l.remove_if(pred)), typename L::size_type);
    TC_ASSERT_EXPR(l.remove_if(pred) == expected);
#else
    ASSERT_SAME_TYPE(decltype(l.remove_if(pred)), void);
    l.remove_if(pred);
#endif
    TC_ASSERT_EXPR(old_size - std::distance(l.begin(), l.end()) == expected);
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

int tc_containers_sequences_forwardlist_forwardlist_ops_remove_if(void) {
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 5, 5, 0, 0, 0, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 4);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 0, 0, 0};
        C c1(std::begin(t1), std::end(t1));
        C c2;
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 4);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 0);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        C c1;
        C c2;
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 0);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == 0);
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5, 0};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 1);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }

    { // LWG issue #526
    int a1[] = {1, 2, 1, 3, 5, 8, 11};
    int a2[] = {   2,    3, 5, 8, 11};
    std::forward_list<PredLWG526> c(a1, a1 + 7);
    do_remove_if(c, std::ref(c.front()), 2);
    for (std::size_t i = 0; i < 5; ++i)
    {
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(c.front() == a2[i]);
        c.pop_front();
    }
    TC_ASSERT_EXPR(c.empty());
    }

#if TEST_STD_VER >= 11
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T, min_allocator<T>> C;
        const T t1[] = {0, 5, 5, 0, 0, 0, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 4);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T, min_allocator<T>> C;
        const T t1[] = {0, 0, 0, 0};
        C c1(std::begin(t1), std::end(t1));
        C c2;
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 4);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T, min_allocator<T>> C;
        const T t1[] = {5, 5, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 0);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T, min_allocator<T>> C;
        C c1;
        C c2;
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 0);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == 0);
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T, min_allocator<T>> C;
        const T t1[] = {5, 5, 5, 0};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        do_remove_if(c1, std::ref(cp), 1);
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
#endif

  return 0;
}
