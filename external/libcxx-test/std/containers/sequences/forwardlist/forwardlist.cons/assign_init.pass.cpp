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

// UNSUPPORTED: c++98, c++03

// <forward_list>

// static int assign(initializer_list<value_type> il);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"
#include <iterator>


int tc_libcxx_containers_forwardlist_cons_assign_init(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {10, 11, 12, 13};
        C c(std::begin(t1), std::end(t1));
        c.assign({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        int n = 0;
        for (C::const_iterator i = c.cbegin(); i != c.cend(); ++i, ++n)
            TC_ASSERT_EXPR(*i == n);
        TC_ASSERT_EXPR(n == 10);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c(std::begin(t1), std::end(t1));
        c.assign({10, 11, 12, 13});
        int n = 0;
        for (C::const_iterator i = c.cbegin(); i != c.cend(); ++i, ++n)
            TC_ASSERT_EXPR(*i == 10+n);
        TC_ASSERT_EXPR(n == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
