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

// class ostream_iterator

// ostream_iterator(const ostream_iterator& x);

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_ostream_iterator_cons_des_copy(void)
{
    std::ostringstream outf;
    std::ostream_iterator<int> i(outf);
    std::ostream_iterator<int> j = i;
    TC_ASSERT_EXPR(outf.good());
    ((void)j);
    TC_SUCCESS_RESULT();
    return 0;
}
