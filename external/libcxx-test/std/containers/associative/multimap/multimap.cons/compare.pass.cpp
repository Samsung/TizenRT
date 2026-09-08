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

// explicit multimap(const key_compare& comp);

// key_compare key_comp() const;

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "test_compare.h"

int tc_libcxx_containers_multimap_cons_compare(void)
{
    {
    typedef test_compare<std::less<int> > C;
    const std::multimap<int, double, C> m(C(3));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
