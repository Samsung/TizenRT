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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <forward_list>

// static int remove(const value_type& v);

#include <forward_list>
#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"


struct S {
    S(int i) : i_(new int(i)) {}
    S(const S &rhs) : i_(new int(*rhs.i_)) {}
    S& operator = (const S &rhs) { *i_ = *rhs.i_; return *this; }
    ~S () { delete i_; i_ = NULL; }
    bool operator == (const S &rhs) const { return *i_ == *rhs.i_; }
    int get () const { return *i_; }
    int *i_;
    };


int tc_libcxx_containers_forwardlist_ops_remove(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 5, 5, 0, 0, 0, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        c1.remove(0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 0, 0, 0};
        C c1(std::begin(t1), std::end(t1));
        C c2;
        c1.remove(0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        c1.remove(0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        C c1;
        C c2;
        c1.remove(0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5, 0};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        c1.remove(0);
        TC_ASSERT_EXPR(c1 == c2);
    }
    {  // LWG issue #526
    typedef int T;
    typedef std::forward_list<T> C;
    int t1[] = {1, 2, 1, 3, 5, 8, 11};
    int t2[] = {   2,    3, 5, 8, 11};
    C c1(std::begin(t1), std::end(t1));
    C c2(std::begin(t2), std::end(t2));
    c1.remove(c1.front());
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
    c.remove(c.front());
    C::const_iterator it = c.begin();
    for(int *ip = std::begin(t2); ip != std::end(t2); ++ip, ++it) {
        TC_ASSERT_EXPR ( it != c.end());
        TC_ASSERT_EXPR ( *ip == it->get());
        }
    TC_ASSERT_EXPR ( it == c.end ());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
