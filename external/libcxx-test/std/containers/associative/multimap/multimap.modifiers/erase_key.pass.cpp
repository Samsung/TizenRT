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

// size_type erase(const key_type& k);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_multimap_modifiers_erase_key(void)
{
    {
        typedef std::multimap<int, double> M;
        typedef std::pair<int, double> P;
        typedef M::size_type I;
        P ar[] =
        {
            P(1, 1),
            P(1, 1.5),
            P(1, 2),
            P(2, 1),
            P(2, 1.5),
            P(2, 2),
            P(3, 1),
            P(3, 1.5),
            P(3, 2),
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 9);
        I i = m.erase(2);
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(i == 3);
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 2);

        i = m.erase(2);
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(i == 0);
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 2);

        i = m.erase(3);
        TC_ASSERT_EXPR(i == 3);
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 2);

        i = m.erase(1);
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == 3);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
