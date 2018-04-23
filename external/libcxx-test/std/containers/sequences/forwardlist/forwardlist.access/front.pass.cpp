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

// reference       front();
// const_reference front() const;

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"
#include <iterator>


int tc_libcxx_containers_forwardlist_access_front(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c(std::begin(t), std::end(t));
        TC_ASSERT_EXPR(c.front() == 0);
        c.front() = 10;
        TC_ASSERT_EXPR(c.front() == 10);
        TC_ASSERT_EXPR(*c.begin() == 10);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        const C c(std::begin(t), std::end(t));
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(*c.begin() == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
