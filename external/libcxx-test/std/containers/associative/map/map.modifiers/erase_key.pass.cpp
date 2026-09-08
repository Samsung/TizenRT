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

// size_type erase(const key_type& k);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_containers_map_modifiers_erase_key(void)
{
    {
        typedef std::map<int, double> M;
        typedef std::pair<int, double> P;
        typedef M::size_type R;
        P ar[] =
        {
            P(1, 1.5),
            P(2, 2.5),
            P(3, 3.5),
            P(4, 4.5),
            P(5, 5.5),
            P(6, 6.5),
            P(7, 7.5),
            P(8, 8.5),
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 8);
        R s = m.erase(9);
        TC_ASSERT_EXPR(s == 0);
        TC_ASSERT_EXPR(m.size() == 8);
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin())->first == 2);
        TC_ASSERT_EXPR(next(m.begin())->second == 2.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 3.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 4);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 4.5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 5.5);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 6);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 6.5);
        TC_ASSERT_EXPR(next(m.begin(), 6)->first == 7);
        TC_ASSERT_EXPR(next(m.begin(), 6)->second == 7.5);
        TC_ASSERT_EXPR(next(m.begin(), 7)->first == 8);
        TC_ASSERT_EXPR(next(m.begin(), 7)->second == 8.5);

        s = m.erase(4);
        TC_ASSERT_EXPR(m.size() == 7);
        TC_ASSERT_EXPR(s == 1);
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin())->first == 2);
        TC_ASSERT_EXPR(next(m.begin())->second == 2.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 3.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 5.5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 6);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 6.5);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 7);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 7.5);
        TC_ASSERT_EXPR(next(m.begin(), 6)->first == 8);
        TC_ASSERT_EXPR(next(m.begin(), 6)->second == 8.5);

        s = m.erase(1);
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(s == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == 2.5);
        TC_ASSERT_EXPR(next(m.begin())->first == 3);
        TC_ASSERT_EXPR(next(m.begin())->second == 3.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 5.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 6);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 6.5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 7);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 7.5);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 8);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 8.5);

        s = m.erase(8);
        TC_ASSERT_EXPR(m.size() == 5);
        TC_ASSERT_EXPR(s == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == 2.5);
        TC_ASSERT_EXPR(next(m.begin())->first == 3);
        TC_ASSERT_EXPR(next(m.begin())->second == 3.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 5.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 6);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 6.5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 7);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 7.5);

        s = m.erase(3);
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(s == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == 2.5);
        TC_ASSERT_EXPR(next(m.begin())->first == 5);
        TC_ASSERT_EXPR(next(m.begin())->second == 5.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 6);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 6.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 7);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 7.5);

        s = m.erase(6);
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(s == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == 2.5);
        TC_ASSERT_EXPR(next(m.begin())->first == 5);
        TC_ASSERT_EXPR(next(m.begin())->second == 5.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 7);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 7.5);

        s = m.erase(7);
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(s == 1);
        TC_ASSERT_EXPR(m.begin()->first == 2);
        TC_ASSERT_EXPR(m.begin()->second == 2.5);
        TC_ASSERT_EXPR(next(m.begin())->first == 5);
        TC_ASSERT_EXPR(next(m.begin())->second == 5.5);

        s = m.erase(2);
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(s == 1);
        TC_ASSERT_EXPR(m.begin()->first == 5);
        TC_ASSERT_EXPR(m.begin()->second == 5.5);

        s = m.erase(5);
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(s == 1);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
