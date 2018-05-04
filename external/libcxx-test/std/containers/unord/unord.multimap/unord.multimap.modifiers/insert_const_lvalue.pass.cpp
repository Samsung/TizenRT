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

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// iterator insert(const value_type& x);

#include <unordered_map>
#include <cassert>
#include "libcxx_tc_common.h"


template<class Container>
static int do_insert_const_lvalue_test()
{
    typedef Container C;
    typedef typename C::iterator R;
    typedef typename C::value_type VT;
    C c;
    const VT v1(3.5, 3);
    R r = c.insert(v1);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(r->first == 3.5);
    TC_ASSERT_EXPR(r->second == 3);

    const VT v2(3.5, 4);
    r = c.insert(v2);
    TC_ASSERT_EXPR(c.size() == 2);
    TC_ASSERT_EXPR(r->first == 3.5);
    TC_ASSERT_EXPR(r->second == 4);

    const VT v3(4.5, 4);
    r = c.insert(v3);
    TC_ASSERT_EXPR(c.size() == 3);
    TC_ASSERT_EXPR(r->first == 4.5);
    TC_ASSERT_EXPR(r->second == 4);

    const VT v4(5.5, 4);
    r = c.insert(v4);
    TC_ASSERT_EXPR(c.size() == 4);
    TC_ASSERT_EXPR(r->first == 5.5);
    TC_ASSERT_EXPR(r->second == 4);
    return 0;
}

int tc_libcxx_containers_unord_multimap_modifiers_insert_const_lvalue(void)
{
    do_insert_const_lvalue_test<std::unordered_multimap<double, int> >();
    TC_SUCCESS_RESULT();
    return 0;
}
