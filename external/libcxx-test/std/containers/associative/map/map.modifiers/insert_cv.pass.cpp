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

// pair<iterator, bool> insert(const value_type& v);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class Container>
static int do_insert_cv_test()
{
    typedef Container M;
    typedef std::pair<typename M::iterator, bool> R;
    typedef typename M::value_type VT;
    M m;

    const VT v1(2, 2.5);
    R r = m.insert(v1);
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(r.first == m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(r.first->first == 2);
    TC_ASSERT_EXPR(r.first->second == 2.5);

    const VT v2(1, 1.5);
    r = m.insert(v2);
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(r.first == m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    TC_ASSERT_EXPR(r.first->first == 1);
    TC_ASSERT_EXPR(r.first->second == 1.5);

    const VT v3(3, 3.5);
    r = m.insert(v3);
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(r.first == prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(r.first->first == 3);
    TC_ASSERT_EXPR(r.first->second == 3.5);

    const VT v4(3, 4.5);
    r = m.insert(v4);
    TC_ASSERT_EXPR(!r.second);
    TC_ASSERT_EXPR(r.first == prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(r.first->first == 3);
    TC_ASSERT_EXPR(r.first->second == 3.5);
    return 0;
}

int tc_libcxx_containers_map_modifiers_insert_cv(void)
{
    do_insert_cv_test<std::map<int, double> >();
    TC_SUCCESS_RESULT();
    return 0;
}
