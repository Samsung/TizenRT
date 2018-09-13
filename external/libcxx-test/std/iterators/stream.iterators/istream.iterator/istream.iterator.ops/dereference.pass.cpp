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

// const T& operator*() const;

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_istream_iterator_ops_dereference(void)
{
    std::istringstream inf(" 1 23");
    std::istream_iterator<int> i(inf);
    int j = 0;
    j = *i;
    TC_ASSERT_EXPR(j == 1);
    j = *i;
    TC_ASSERT_EXPR(j == 1);
    ++i;
    j = *i;
    TC_ASSERT_EXPR(j == 23);
    j = *i;
    TC_ASSERT_EXPR(j == 23);
    TC_SUCCESS_RESULT();
    return 0;
}
