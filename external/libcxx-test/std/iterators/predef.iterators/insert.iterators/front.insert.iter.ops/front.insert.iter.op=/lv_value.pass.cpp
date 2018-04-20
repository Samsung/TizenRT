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

// front_insert_iterator

// front_insert_iterator<Cont>&
//   operator=(const Cont::value_type& value);

#include <iterator>
#include <list>
#include <cassert>
#include "libcxx_tc_common.h"
#include "nasty_containers.hpp"

template <class C>
static int
test(C c)
{
    const typename C::value_type v = typename C::value_type();
    std::front_insert_iterator<C> i(c);
    i = v;
    TC_ASSERT_EXPR(c.front() == v);
    return 0;
}

class Copyable
{
    int data_;
public:
    Copyable() : data_(0) {}
    ~Copyable() {data_ = -1;}

    friend bool operator==(const Copyable& x, const Copyable& y)
        {return x.data_ == y.data_;}
};

int tc_libcxx_iterators_front_insert_iter_op__lv_value(void)
{
    TC_ASSERT_FUNC((test(std::list<Copyable>())));
    TC_ASSERT_FUNC((test(nasty_list<Copyable>())));
    TC_SUCCESS_RESULT();
    return 0;
}
