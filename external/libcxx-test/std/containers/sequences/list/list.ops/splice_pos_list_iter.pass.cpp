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

// <list>

// static int splice(const_iterator position, list<T,Allocator>& x, iterator i);

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_containers_list_ops_splice_pos_list_iter(void)
{
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5, 6};
    {
        std::list<int> l1;
        std::list<int> l2(a2, a2+1);
        l1.splice(l1.end(), l2, l2.begin());
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 1);
        TC_ASSERT_EXPR(l2.size() == 0);
        TC_ASSERT_EXPR(distance(l2.begin(), l2.end()) == 0);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int> l1;
        std::list<int> l2(a2, a2+2);
        l1.splice(l1.end(), l2, l2.begin());
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 1);
        TC_ASSERT_EXPR(l2.size() == 1);
        TC_ASSERT_EXPR(distance(l2.begin(), l2.end()) == 1);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 4);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 5);
    }
    {
        std::list<int> l1;
        std::list<int> l2(a2, a2+2);
        l1.splice(l1.end(), l2, next(l2.begin()));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 1);
        TC_ASSERT_EXPR(l2.size() == 1);
        TC_ASSERT_EXPR(distance(l2.begin(), l2.end()) == 1);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 5);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int> l1;
        std::list<int> l2(a2, a2+3);
        l1.splice(l1.end(), l2, l2.begin());
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 1);
        TC_ASSERT_EXPR(l2.size() == 2);
        TC_ASSERT_EXPR(distance(l2.begin(), l2.end()) == 2);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 4);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
    }
    {
        std::list<int> l1;
        std::list<int> l2(a2, a2+3);
        l1.splice(l1.end(), l2, next(l2.begin()));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 1);
        TC_ASSERT_EXPR(l2.size() == 2);
        TC_ASSERT_EXPR(distance(l2.begin(), l2.end()) == 2);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 5);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
    }
    {
        std::list<int> l1;
        std::list<int> l2(a2, a2+3);
        l1.splice(l1.end(), l2, next(l2.begin(), 2));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 1);
        TC_ASSERT_EXPR(l2.size() == 2);
        TC_ASSERT_EXPR(distance(l2.begin(), l2.end()) == 2);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 6);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
        ++i;
        TC_ASSERT_EXPR(*i == 5);
    }
    {
        std::list<int> l1(a1, a1+1);
        l1.splice(l1.begin(), l1, l1.begin());
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 1);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
    }
    {
        std::list<int> l1(a1, a1+1);
        std::list<int> l2(a2, a2+1);
        l1.splice(l1.begin(), l2, l2.begin());
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 2);
        TC_ASSERT_EXPR(l2.size() == 0);
        TC_ASSERT_EXPR(distance(l2.begin(), l2.end()) == 0);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 4);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
    }
    {
        std::list<int> l1(a1, a1+1);
        std::list<int> l2(a2, a2+1);
        l1.splice(next(l1.begin()), l2, l2.begin());
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 2);
        TC_ASSERT_EXPR(l2.size() == 0);
        TC_ASSERT_EXPR(distance(l2.begin(), l2.end()) == 0);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int> l1(a1, a1+2);
        l1.splice(l1.begin(), l1, l1.begin());
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 2);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
    }
    {
        std::list<int> l1(a1, a1+2);
        l1.splice(l1.begin(), l1, next(l1.begin()));
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 2);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
    }
    {
        std::list<int> l1(a1, a1+2);
        l1.splice(next(l1.begin()), l1, l1.begin());
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 2);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
    }
    {
        std::list<int> l1(a1, a1+2);
        l1.splice(next(l1.begin()), l1, next(l1.begin()));
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 2);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
