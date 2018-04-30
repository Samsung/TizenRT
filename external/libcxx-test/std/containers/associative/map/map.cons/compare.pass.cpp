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

// <map>

// class map

// explicit map(const key_compare& comp);

// key_compare key_comp() const;

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_compare.h"

int tc_libcxx_containers_map_cons_compare(void)
{
    {
    typedef test_compare<std::less<int> > C;
    const std::map<int, double, C> m(C(3));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
