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

// <vector>
// vector<bool>

// iterator erase(const_iterator position);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"


int tc_libcxx_containers_vector_bool_erase_iter(void)
{
    bool a1[] = {1, 0, 1};
    {
    std::vector<bool> l1(a1, a1+3);
    std::vector<bool>::const_iterator i = l1.begin();
    ++i;
    std::vector<bool>::iterator j = l1.erase(i);
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 2);
    TC_ASSERT_EXPR(*j == true);
    TC_ASSERT_EXPR(*l1.begin() == 1);
    TC_ASSERT_EXPR(*next(l1.begin()) == true);
    j = l1.erase(j);
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 1);
    TC_ASSERT_EXPR(*l1.begin() == true);
    j = l1.erase(l1.begin());
    TC_ASSERT_EXPR(j == l1.end());
    TC_ASSERT_EXPR(l1.size() == 0);
    TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
