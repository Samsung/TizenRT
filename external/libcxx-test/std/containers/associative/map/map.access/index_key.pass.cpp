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

// mapped_type& operator[](const key_type& k);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "private_constructor.hpp"
#if TEST_STD_VER >= 11
#include "container_test_types.h"
#endif

int tc_libcxx_containers_map_access_index_key(void)
{
    {
    typedef std::pair<const int, double> V;
    V ar[] =
    {
        V(1, 1.5),
        V(2, 2.5),
        V(3, 3.5),
        V(4, 4.5),
        V(5, 5.5),
        V(7, 7.5),
        V(8, 8.5),
    };
    std::map<int, double> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
    TC_ASSERT_EXPR(m.size() == 7);
    TC_ASSERT_EXPR(m[1] == 1.5);
    TC_ASSERT_EXPR(m.size() == 7);
    m[1] = -1.5;
    TC_ASSERT_EXPR(m[1] == -1.5);
    TC_ASSERT_EXPR(m.size() == 7);
    TC_ASSERT_EXPR(m[6] == 0);
    TC_ASSERT_EXPR(m.size() == 8);
    m[6] = 6.5;
    TC_ASSERT_EXPR(m[6] == 6.5);
    TC_ASSERT_EXPR(m.size() == 8);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
