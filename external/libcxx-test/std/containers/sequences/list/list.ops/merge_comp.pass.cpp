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

// <list>

// template <class Compare> static int merge(list& x, Compare comp);

#include <list>
#include <functional>
#include <cassert>
#include "libcxx_tc_common.h"


int tc_libcxx_containers_list_ops_merge_comp(void)
{
    {
    int a1[] = {10, 9, 7, 3, 1};
    int a2[] = {11, 8, 6, 5, 4, 2, 0};
    int a3[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    std::list<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
    std::list<int> c2(a2, a2+sizeof(a2)/sizeof(a2[0]));
    c1.merge(c2, std::greater<int>());
    TC_ASSERT_EXPR(c1 == std::list<int>(a3, a3+sizeof(a3)/sizeof(a3[0])));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
