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

// UNSUPPORTED: c++98, c++03

// <map>

// class map

// map(initializer_list<value_type> il, const key_compare& comp, const allocator_type& a);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "test_compare.h"
#include "test_allocator.h"

int tc_libcxx_containers_map_cons_initializer_list_compare_alloc(void)
{
    {
    typedef std::pair<const int, double> V;
    typedef test_compare<std::less<int> > C;
    typedef test_allocator<std::pair<const int, double> > A;
    std::map<int, double, C, A> m({
                                   {1, 1},
                                   {1, 1.5},
                                   {1, 2},
                                   {2, 1},
                                   {2, 1.5},
                                   {2, 2},
                                   {3, 1},
                                   {3, 1.5},
                                   {3, 2}
                                  }, C(3), A(6));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*next(m.begin(), 2) == V(3, 1));
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    TC_ASSERT_EXPR(m.get_allocator() == A(6));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
