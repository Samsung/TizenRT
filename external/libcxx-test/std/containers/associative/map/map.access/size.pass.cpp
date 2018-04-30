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

// size_type size() const;

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"


int tc_libcxx_containers_map_access_size(void)
{
    {
    typedef std::map<int, double> M;
    M m;
    TC_ASSERT_EXPR(m.size() == 0);
    m.insert(M::value_type(2, 1.5));
    TC_ASSERT_EXPR(m.size() == 1);
    m.insert(M::value_type(1, 1.5));
    TC_ASSERT_EXPR(m.size() == 2);
    m.insert(M::value_type(3, 1.5));
    TC_ASSERT_EXPR(m.size() == 3);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    m.erase(m.begin());
    TC_ASSERT_EXPR(m.size() == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
