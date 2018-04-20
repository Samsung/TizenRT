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

// <iterator>

// insert_iterator

// requires CopyConstructible<Cont::value_type>
//   insert_iterator<Cont>&
//   operator=(const Cont::value_type& value);

#include <iterator>
#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"
#include "nasty_containers.hpp"

template <class C>
static int
test(C c1, typename C::difference_type j,
     typename C::value_type x1, typename C::value_type x2,
     typename C::value_type x3, const C& c2)
{
    std::insert_iterator<C> q(c1, c1.begin() + j);
    q = x1;
    q = x2;
    q = x3;
    TC_ASSERT_EXPR(c1 == c2);
    return 0;
}

template <class C>
static int
insert3at(C& c, typename C::iterator i,
     typename C::value_type x1, typename C::value_type x2,
     typename C::value_type x3)
{
    i = c.insert(i, x1);
    i = c.insert(++i, x2);
    c.insert(++i, x3);
    return 0;
}

int tc_libcxx_iterators_insert_iter_op__lv_value(void)
{
    {
    typedef std::vector<int> C;
    C c1;
    for (int i = 0; i < 3; ++i)
        c1.push_back(i);
    C c2 = c1;
    insert3at(c2, c2.begin(), 'a', 'b', 'c');
    TC_ASSERT_FUNC((test(c1, 0, 'a', 'b', 'c', c2)));
    c2 = c1;
    insert3at(c2, c2.begin()+1, 'a', 'b', 'c');
    TC_ASSERT_FUNC((test(c1, 1, 'a', 'b', 'c', c2)));
    c2 = c1;
    insert3at(c2, c2.begin()+2, 'a', 'b', 'c');
    TC_ASSERT_FUNC((test(c1, 2, 'a', 'b', 'c', c2)));
    c2 = c1;
    insert3at(c2, c2.begin()+3, 'a', 'b', 'c');
    TC_ASSERT_FUNC((test(c1, 3, 'a', 'b', 'c', c2)));
    }
    {
    typedef nasty_vector<int> C;
    C c1;
    for (int i = 0; i < 3; ++i)
        c1.push_back(i);
    C c2 = c1;
    insert3at(c2, c2.begin(), 'a', 'b', 'c');
    TC_ASSERT_FUNC((test(c1, 0, 'a', 'b', 'c', c2)));
    c2 = c1;
    insert3at(c2, c2.begin()+1, 'a', 'b', 'c');
    TC_ASSERT_FUNC((test(c1, 1, 'a', 'b', 'c', c2)));
    c2 = c1;
    insert3at(c2, c2.begin()+2, 'a', 'b', 'c');
    TC_ASSERT_FUNC((test(c1, 2, 'a', 'b', 'c', c2)));
    c2 = c1;
    insert3at(c2, c2.begin()+3, 'a', 'b', 'c');
    TC_ASSERT_FUNC((test(c1, 3, 'a', 'b', 'c', c2)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
