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

// template <InputIterator Iter>
//   move_iterator<Iter>
//   make_move_iterator(const Iter& i);
//
//  constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
static int
test(It i)
{
    const std::move_iterator<It> r(i);
    TC_ASSERT_EXPR(std::make_move_iterator(i) == r);
    return 0;
}

int tc_libcxx_iterators_move_iter_nonmember_make_move_iterator(void)
{
    {
    char s[] = "1234567890";
    TC_ASSERT_FUNC((test(input_iterator<char*>(s+5))));
    TC_ASSERT_FUNC((test(forward_iterator<char*>(s+5))));
    TC_ASSERT_FUNC((test(bidirectional_iterator<char*>(s+5))));
    TC_ASSERT_FUNC((test(random_access_iterator<char*>(s+5))));
    TC_ASSERT_FUNC((test(s+5)));
    }
    {
    int a[] = {1,2,3,4};
    TEST_IGNORE_NODISCARD std::make_move_iterator(a+4);
    TEST_IGNORE_NODISCARD std::make_move_iterator(a); // test for LWG issue 2061
    }

#if TEST_STD_VER > 14
    {
    constexpr const char *p = "123456789";
    constexpr auto iter = std::make_move_iterator<const char *>(p);
    static_assert(iter.base() == p);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
