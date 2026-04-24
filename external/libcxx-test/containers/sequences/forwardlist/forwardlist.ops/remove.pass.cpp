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

// void remove(const value_type& v);      // C++17 and before
// size_type remove(const value_type& v); // C++20 and after

#include <forward_list>
#include <iterator>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class L>
void do_remove(L &l, const typename L::value_type &value, typename L::size_type expected)
{
    typename L::size_type old_size = std::distance(l.begin(), l.end());
#if TEST_STD_VER > 17
    ASSERT_SAME_TYPE(decltype(l.remove(value)), typename L::size_type);
    TC_ASSERT_EXPR(l.remove(value) == expected);
#else
    ASSERT_SAME_TYPE(decltype(l.remove(value)), void);
    l.remove(value);
#endif
    TC_ASSERT_EXPR(old_size - std::distance(l.begin(), l.end()) == expected);
}


struct S {
    S(int i) : i_(new int(i)) {}
    S(const S &rhs) : i_(new int(*rhs.i_)) {}
    S& operator = (const S &rhs) { *i_ = *rhs.i_; return *this; }
    ~S () { delete i_; i_ = NULL; }
    bool operator == (const S &rhs) const { return *i_ == *rhs.i_; }
    int get () const { return *i_; }
    int *i_;
    };


int tc_containers_sequences_forwardlist_forwardlist_ops_remove(void) {
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 5, 5, 0, 0, 0, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        do_remove(c1, 0, 4);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 0, 0, 0};
        C c1(std::begin(t1), std::end(t1));
        C c2;
        do_remove(c1, 0, 4);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        do_remove(c1, 0, 0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        C c1;
        C c2;
        do_remove(c1, 0, 0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5, 0};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        do_remove(c1, 0, 1);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {  // LWG issue #526
    typedef int T;
    typedef std::forward_list<T> C;
    int t1[] = {1, 2, 1, 3, 5, 8, 11};
    int t2[] = {   2,    3, 5, 8, 11};
    C c1(std::begin(t1), std::end(t1));
    C c2(std::begin(t2), std::end(t2));
    do_remove(c1, c1.front(), 2);
    TC_ASSERT_EXPR(c1 == c2);
    }
    {
    typedef S T;
    typedef std::forward_list<T> C;
    int t1[] = {1, 2, 1, 3, 5, 8, 11, 1};
    int t2[] = {   2,    3, 5, 8, 11   };
    C c;
    for(int *ip = std::end(t1); ip != std::begin(t1);)
        c.push_front(S(*--ip));
    do_remove(c, c.front(), 3);
    C::const_iterator it = c.begin();
    for(int *ip = std::begin(t2); ip != std::end(t2); ++ip, ++it) {
        TC_ASSERT_EXPR( it != c.end());
        TC_ASSERT_EXPR( *ip == it->get());
        }
    TC_ASSERT_EXPR( it == c.end ());
    }
#if TEST_STD_VER >= 11
    {
        typedef int T;
        typedef std::forward_list<T, min_allocator<T>> C;
        const T t1[] = {0, 5, 5, 0, 0, 0, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        do_remove(c1, 0, 4);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T, min_allocator<T>> C;
        const T t1[] = {0, 0, 0, 0};
        C c1(std::begin(t1), std::end(t1));
        C c2;
        do_remove(c1, 0, 4);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T, min_allocator<T>> C;
        const T t1[] = {5, 5, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        do_remove(c1, 0, 0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T, min_allocator<T>> C;
        C c1;
        C c2;
        do_remove(c1, 0, 0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T, min_allocator<T>> C;
        const T t1[] = {5, 5, 5, 0};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        do_remove(c1, 0, 1);
        TC_ASSERT_EXPR(c1 == c2);
    }
#endif

  return 0;
}
