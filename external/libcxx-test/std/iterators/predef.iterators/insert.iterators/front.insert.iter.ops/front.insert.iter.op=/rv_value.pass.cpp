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

// UNSUPPORTED: c++98, c++03

// <iterator>

// front_insert_iterator

// front_insert_iterator<Cont>&
//   operator=(Cont::value_type&& value);

#include <iterator>
#include <list>
#include <memory>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class C>
static int
test(C c)
{
    std::front_insert_iterator<C> i(c);
    i = typename C::value_type();
    TC_ASSERT_EXPR(c.front() == typename C::value_type());
    return 0;
}

int tc_libcxx_iterators_front_insert_iter_op__rv_value(void)
{
    TC_ASSERT_FUNC((test(std::list<std::unique_ptr<int> >())));
    TC_SUCCESS_RESULT();
    return 0;
}
