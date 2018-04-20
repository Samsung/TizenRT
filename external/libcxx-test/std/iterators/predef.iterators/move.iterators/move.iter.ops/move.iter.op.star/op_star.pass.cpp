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

// move_iterator

// reference operator*() const;
//
//  constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"
#include <memory>

#include "test_macros.h"

class A
{
    int data_;
public:
    A() : data_(1) {}
    ~A() {data_ = -1;}

    friend bool operator==(const A& x, const A& y)
        {return x.data_ == y.data_;}
};

template <class It>
static int
test(It i, typename std::iterator_traits<It>::value_type x)
{
    std::move_iterator<It> r(i);
    TC_ASSERT_EXPR(*r == x);
    typename std::iterator_traits<It>::value_type x2 = *r;
    TC_ASSERT_EXPR(x2 == x);
    return 0;
}

struct do_nothing
{
    void operator()(void*) const {}
};


int tc_libcxx_iterators_move_iter_op_star_op_star(void)
{
    {
        A a;
        test(&a, A());
    }
#if TEST_STD_VER >= 11
    {
        int i;
        std::unique_ptr<int, do_nothing> p(&i);
        test(&p, std::unique_ptr<int, do_nothing>(&i));
    }
#endif
#if TEST_STD_VER > 14
    {
    constexpr const char *p = "123456789";
    typedef std::move_iterator<const char *> MI;
    constexpr MI it1 = std::make_move_iterator(p);
    constexpr MI it2 = std::make_move_iterator(p+1);
    static_assert(*it1 == p[0], "");
    static_assert(*it2 == p[1], "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
