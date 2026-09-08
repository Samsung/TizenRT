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

// iterator       before_begin();
// const_iterator before_begin() const;
// const_iterator cbefore_begin() const;

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include <iterator>


int tc_libcxx_containers_forwardlist_iter_before_begin(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        C c;
        C::iterator i = c.before_begin();
        TC_ASSERT_EXPR(std::distance(i, c.end()) == 1);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const C c;
        C::const_iterator i = c.before_begin();
        TC_ASSERT_EXPR(std::distance(i, c.end()) == 1);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const C c;
        C::const_iterator i = c.cbefore_begin();
        TC_ASSERT_EXPR(std::distance(i, c.end()) == 1);
        TC_ASSERT_EXPR(c.cbefore_begin() == c.before_begin());
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c(std::begin(t), std::end(t));
        C::iterator i = c.before_begin();
        TC_ASSERT_EXPR(std::distance(i, c.end()) == 11);
        TC_ASSERT_EXPR(std::next(c.before_begin()) == c.begin());
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        const C c(std::begin(t), std::end(t));
        C::const_iterator i = c.before_begin();
        TC_ASSERT_EXPR(std::distance(i, c.end()) == 11);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
