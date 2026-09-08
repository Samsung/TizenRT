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

// <map>

// class map

// map(const map& m, const allocator_type& a);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_compare.h"
#include "test_allocator.h"

int tc_libcxx_containers_map_cons_copy_alloc(void)
{
    {
    typedef std::pair<const int, double> V;
    V ar[] =
    {
        V(1, 1),
        V(1, 1.5),
        V(1, 2),
        V(2, 1),
        V(2, 1.5),
        V(2, 2),
        V(3, 1),
        V(3, 1.5),
        V(3, 2),
    };
    typedef test_compare<std::less<int> > C;
    typedef test_allocator<V> A;
    std::map<int, double, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(7));
    std::map<int, double, C, A> m(mo, A(3));
    TC_ASSERT_EXPR(m.get_allocator() == A(3));
    TC_ASSERT_EXPR(m.key_comp() == C(5));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*next(m.begin(), 2) == V(3, 1));

    TC_ASSERT_EXPR(mo.get_allocator() == A(7));
    TC_ASSERT_EXPR(mo.key_comp() == C(5));
    TC_ASSERT_EXPR(mo.size() == 3);
    TC_ASSERT_EXPR(distance(mo.begin(), mo.end()) == 3);
    TC_ASSERT_EXPR(*mo.begin() == V(1, 1));
    TC_ASSERT_EXPR(*next(mo.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*next(mo.begin(), 2) == V(3, 1));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
