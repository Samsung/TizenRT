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

// move_iterator();
//
//  constexpr in C++17

#include <iterator>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

template <class It>
static int
test()
{
    std::move_iterator<It> r;
    (void)r;
    return 0;
}

int tc_libcxx_iterators_move_iter_op_const_default(void)
{
    TC_ASSERT_FUNC((test<input_iterator<char*> >()));
    TC_ASSERT_FUNC((test<forward_iterator<char*> >()));
    TC_ASSERT_FUNC((test<bidirectional_iterator<char*> >()));
    TC_ASSERT_FUNC((test<random_access_iterator<char*> >()));
    TC_ASSERT_FUNC((test<char*>()));

#if TEST_STD_VER > 14
    {
    constexpr std::move_iterator<const char *> it;
    (void)it;
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
