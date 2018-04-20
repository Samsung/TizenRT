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

// template <class T, size_t N> T* begin(T (&array)[N]);

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_iterator_range_begin_array(void)
{
    int ia[] = {1, 2, 3};
    int* i = std::begin(ia);
    TC_ASSERT_EXPR(*i == 1);
    *i = 2;
    TC_ASSERT_EXPR(ia[0] == 2);
    TC_SUCCESS_RESULT();
    return 0;
}
