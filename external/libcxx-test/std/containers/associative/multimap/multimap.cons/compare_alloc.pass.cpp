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

// multimap(const key_compare& comp, const allocator_type& a);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "test_compare.h"
#include "test_allocator.h"

int tc_libcxx_containers_multimap_cons_compare_alloc(void)
{
    {
    typedef test_compare<std::less<int> > C;
    typedef test_allocator<std::pair<const int, double> > A;
    std::multimap<int, double, C, A> m(C(4), A(5));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.key_comp() == C(4));
    TC_ASSERT_EXPR(m.get_allocator() == A(5));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
