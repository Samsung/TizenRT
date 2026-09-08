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

// class multimap

// multimap(const multimap& m);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_compare.h"
#include "test_allocator.h"

int tc_libcxx_containers_multimap_cons_copy(void)
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
        std::multimap<int, double, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(7));
        std::multimap<int, double, C, A> m = mo;
        TC_ASSERT_EXPR(m == mo);
        TC_ASSERT_EXPR(m.get_allocator() == A(7));
        TC_ASSERT_EXPR(m.key_comp() == C(5));

        TC_ASSERT_EXPR(mo.get_allocator() == A(7));
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
    }
#if TEST_STD_VER >= 11
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
        typedef other_allocator<V> A;
        std::multimap<int, double, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(7));
        std::multimap<int, double, C, A> m = mo;
        TC_ASSERT_EXPR(m == mo);
        TC_ASSERT_EXPR(m.get_allocator() == A(-2));
        TC_ASSERT_EXPR(m.key_comp() == C(5));

        TC_ASSERT_EXPR(mo.get_allocator() == A(7));
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
