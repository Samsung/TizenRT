//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
// class unordered_map

// iterator insert(const_iterator p, const value_type& x);

#include <unordered_map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template<class Container>
static int do_insert_hint_const_lvalue_test()
{
    typedef Container C;
    typedef typename C::iterator R;
    typedef typename C::value_type VT;
    C c;
    typename C::const_iterator e = c.end();
    const VT v1(3.5, 3);
    R r = c.insert(e, v1);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(r->first == 3.5);
    TC_ASSERT_EXPR(r->second == 3);

    const VT v2(3.5, 4);
    r = c.insert(c.end(), v2);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(r->first == 3.5);
    TC_ASSERT_EXPR(r->second == 3);

    const VT v3(4.5, 4);
    r = c.insert(c.end(), v3);
    TC_ASSERT_EXPR(c.size() == 2);
    TC_ASSERT_EXPR(r->first == 4.5);
    TC_ASSERT_EXPR(r->second == 4);

    const VT v4(5.5, 4);
    r = c.insert(c.end(), v4);
    TC_ASSERT_EXPR(c.size() == 3);
    TC_ASSERT_EXPR(r->first == 5.5);
    TC_ASSERT_EXPR(r->second == 4);
    return 0;
}

int tc_libcxx_containers_unord_map_modifiers_insert_hint_const_lvalue(void)
{
    do_insert_hint_const_lvalue_test<std::unordered_map<double, int> >();
    TC_SUCCESS_RESULT();
    return 0;
}
