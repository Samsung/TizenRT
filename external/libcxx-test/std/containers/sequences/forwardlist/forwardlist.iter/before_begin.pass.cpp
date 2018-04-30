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

// iterator       before_begin();
// const_iterator before_begin() const;
// const_iterator cbefore_begin() const;

#include <forward_list>
#include <cassert>
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
