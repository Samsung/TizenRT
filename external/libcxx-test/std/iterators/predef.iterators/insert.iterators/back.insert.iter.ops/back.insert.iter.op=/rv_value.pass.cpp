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

// back_insert_iterator

// requires CopyConstructible<Cont::value_type>
//   back_insert_iterator<Cont>&
//   operator=(Cont::value_type&& value);

#include <iterator>

#include <vector>
#include <memory>
#include <cassert>
#include "libcxx_tc_common.h"

template <class C>
static int
test(C c)
{
    std::back_insert_iterator<C> i(c);
    i = typename C::value_type();
    TC_ASSERT_EXPR(c.back() == typename C::value_type());
    return 0;
}

int tc_libcxx_iterators_back_insert_iter_op__rv_value(void)
{
    TC_ASSERT_FUNC((test(std::vector<std::unique_ptr<int> >())));
    TC_SUCCESS_RESULT();
    return 0;
}
