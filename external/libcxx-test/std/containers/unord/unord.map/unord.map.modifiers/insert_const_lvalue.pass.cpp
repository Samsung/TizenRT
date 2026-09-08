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

// pair<iterator, bool> insert(const value_type& x);

#include <unordered_map>
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

    const VT v1(2.5, 2);
    R r = m.insert(v1);
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(r.first->first == 2.5);
    TC_ASSERT_EXPR(r.first->second == 2);

    const VT v2(2.5, 3);
    r = m.insert(v2);
    TC_ASSERT_EXPR(!r.second);
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(r.first->first == 2.5);
    TC_ASSERT_EXPR(r.first->second == 2);

    const VT v3(1.5, 1);
    r = m.insert(v3);
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(m.size() == 2);
    TC_ASSERT_EXPR(r.first->first == 1.5);
    TC_ASSERT_EXPR(r.first->second == 1);

    const VT v4(3.5, 3);
    r = m.insert(v4);
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(r.first->first == 3.5);
    TC_ASSERT_EXPR(r.first->second == 3);

    const VT v5(3.5, 4);
    r = m.insert(v5);
    TC_ASSERT_EXPR(!r.second);
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(r.first->first == 3.5);
    TC_ASSERT_EXPR(r.first->second == 3);
    return 0;
}

int tc_libcxx_containers_unord_map_modifiers_insert_const_lvalue(void)
{
    {
        typedef std::unordered_map<double, int> M;
        do_insert_cv_test<M>();
    }
    TC_SUCCESS_RESULT();
    return 0;
}
