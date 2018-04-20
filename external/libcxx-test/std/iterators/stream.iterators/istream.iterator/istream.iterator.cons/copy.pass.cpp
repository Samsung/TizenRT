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

// class istream_iterator

// istream_iterator(const istream_iterator& x);
//  C++17 says:  If is_trivially_copy_constructible_v<T> is true, then
//     this constructor is a trivial copy constructor.

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_iterators_istream_iterator_cons_copy(void)
{
    {
        std::istream_iterator<int> io;
        std::istream_iterator<int> i = io;
        TC_ASSERT_EXPR(i == std::istream_iterator<int>());
    }
    {
        std::istringstream inf(" 1 23");
        std::istream_iterator<int> io(inf);
        std::istream_iterator<int> i = io;
        TC_ASSERT_EXPR(i != std::istream_iterator<int>());
        int j = 0;
        j = *i;
        TC_ASSERT_EXPR(j == 1);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
