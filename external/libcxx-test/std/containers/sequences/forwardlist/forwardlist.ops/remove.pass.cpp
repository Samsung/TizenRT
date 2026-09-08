//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
#include "test_macros.h"
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
