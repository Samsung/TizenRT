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

// template <class InputIterator>
//     multimap(InputIterator first, InputIterator last,
//              const key_compare& comp);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "test_compare.h"

int tc_libcxx_containers_multimap_cons_iter_iter_comp(void)
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
    std::multimap<int, double, C> m(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5));
    TC_ASSERT_EXPR(m.key_comp() == C(5));
    TC_ASSERT_EXPR(m.size() == 9);
    TC_ASSERT_EXPR(distance(m.begin(), m.end()) == 9);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*next(m.begin()) == V(1, 1.5));
    TC_ASSERT_EXPR(*next(m.begin(), 2) == V(1, 2));
    TC_ASSERT_EXPR(*next(m.begin(), 3) == V(2, 1));
    TC_ASSERT_EXPR(*next(m.begin(), 4) == V(2, 1.5));
    TC_ASSERT_EXPR(*next(m.begin(), 5) == V(2, 2));
    TC_ASSERT_EXPR(*next(m.begin(), 6) == V(3, 1));
    TC_ASSERT_EXPR(*next(m.begin(), 7) == V(3, 1.5));
    TC_ASSERT_EXPR(*next(m.begin(), 8) == V(3, 2));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
