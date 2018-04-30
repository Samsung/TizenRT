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

// UNSUPPORTED: c++98, c++03

// <map>

// class map

// mapped_type& operator[](key_type&& k);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "MoveOnly.h"
#include "container_test_types.h"

int tc_libcxx_containers_map_access_index_rv_key(void)
{
    std::map<MoveOnly, double> m;
    TC_ASSERT_EXPR(m.size() == 0);
    TC_ASSERT_EXPR(m[1] == 0.0);
    TC_ASSERT_EXPR(m.size() == 1);
    m[1] = -1.5;
    TC_ASSERT_EXPR(m[1] == -1.5);
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(m[6] == 0);
    TC_ASSERT_EXPR(m.size() == 2);
    m[6] = 6.5;
    TC_ASSERT_EXPR(m[6] == 6.5);
    TC_ASSERT_EXPR(m.size() == 2);
    TC_SUCCESS_RESULT();
    return 0;
}
