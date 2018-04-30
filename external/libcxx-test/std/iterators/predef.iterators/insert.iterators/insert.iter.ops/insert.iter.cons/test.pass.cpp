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

// insert_iterator(Cont& x, Cont::iterator i);

#include <iterator>
#include <vector>
#include "nasty_containers.hpp"
#include "libcxx_tc_common.h"

template <class C>
static int
test(C c)
{
    std::insert_iterator<C> i(c, c.begin());
    return 0;
}

int tc_libcxx_iterators_insert_iter_cons_test(void)
{
    TC_ASSERT_FUNC((test(std::vector<int>())));
    TC_ASSERT_FUNC((test(nasty_vector<int>())));
    TC_SUCCESS_RESULT();
    return 0;
}
