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

// <iterator>

// insert_iterator

// requires CopyConstructible<Cont::value_type>
//   insert_iterator<Cont>&
//   operator=(const Cont::value_type& value);

#include <iterator>

#include <utility>
#include <vector>
#include <memory>
#include <cassert>
#include "libcxx_tc_common.h"

template <class C>
static int
test(C c1, typename C::difference_type j,
     typename C::value_type x1, typename C::value_type x2,
     typename C::value_type x3, const C& c2)
{
    std::insert_iterator<C> q(c1, c1.begin() + j);
    q = std::move(x1);
    q = std::move(x2);
    q = std::move(x3);
    TC_ASSERT_EXPR(c1 == c2);
    return 0;
}

template <class C>
static int
insert3at(C& c, typename C::iterator i,
     typename C::value_type x1, typename C::value_type x2,
     typename C::value_type x3)
{
    i = c.insert(i, std::move(x1));
    i = c.insert(++i, std::move(x2));
    c.insert(++i, std::move(x3));
    return 0;
}

struct do_nothing
{
    void operator()(void*) const {}
};

int tc_libcxx_iterators_insert_iter_op__rv_value(void)
{
    {
    typedef std::unique_ptr<int, do_nothing> Ptr;
    typedef std::vector<Ptr> C;
    C c1;
    int x[6] = {0};
    for (int i = 0; i < 3; ++i)
        c1.push_back(Ptr(x+i));
    C c2;
    for (int i = 0; i < 3; ++i)
        c2.push_back(Ptr(x+i));
    insert3at(c2, c2.begin(), Ptr(x+3), Ptr(x+4), Ptr(x+5));
    TC_ASSERT_FUNC((test(std::move(c1), 0, Ptr(x+3), Ptr(x+4), Ptr(x+5), c2)));
    c1.clear();
    for (int i = 0; i < 3; ++i)
        c1.push_back(Ptr(x+i));
    c2.clear();
    for (int i = 0; i < 3; ++i)
        c2.push_back(Ptr(x+i));
    insert3at(c2, c2.begin()+1, Ptr(x+3), Ptr(x+4), Ptr(x+5));
    TC_ASSERT_FUNC((test(std::move(c1), 1, Ptr(x+3), Ptr(x+4), Ptr(x+5), c2)));
    c1.clear();
    for (int i = 0; i < 3; ++i)
        c1.push_back(Ptr(x+i));
    c2.clear();
    for (int i = 0; i < 3; ++i)
        c2.push_back(Ptr(x+i));
    insert3at(c2, c2.begin()+2, Ptr(x+3), Ptr(x+4), Ptr(x+5));
    TC_ASSERT_FUNC((test(std::move(c1), 2, Ptr(x+3), Ptr(x+4), Ptr(x+5), c2)));
    c1.clear();
    for (int i = 0; i < 3; ++i)
        c1.push_back(Ptr(x+i));
    c2.clear();
    for (int i = 0; i < 3; ++i)
        c2.push_back(Ptr(x+i));
    insert3at(c2, c2.begin()+3, Ptr(x+3), Ptr(x+4), Ptr(x+5));
    TC_ASSERT_FUNC((test(std::move(c1), 3, Ptr(x+3), Ptr(x+4), Ptr(x+5), c2)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
