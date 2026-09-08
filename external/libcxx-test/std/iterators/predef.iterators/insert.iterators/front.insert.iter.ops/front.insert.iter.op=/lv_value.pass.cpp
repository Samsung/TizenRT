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

// <iterator>

// front_insert_iterator

// front_insert_iterator<Cont>&
//   operator=(const Cont::value_type& value);

#include <iterator>
#include <list>
#include <cassert>
#include "test_macros.h"
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
