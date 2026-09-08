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

//       mapped_type& at(const key_type& k);
// const mapped_type& at(const key_type& k) const;

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_containers_map_access_at(void)
{
    {
        typedef std::pair<const int, double> V;
        V ar[] =
        {
            V(1, 1.5),
            V(2, 2.5),
            V(3, 3.5),
            V(4, 4.5),
            V(5, 5.5),
            V(7, 7.5),
            V(8, 8.5),
        };
        std::map<int, double> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 7);
        TC_ASSERT_EXPR(m.at(1) == 1.5);
        m.at(1) = -1.5;
        TC_ASSERT_EXPR(m.at(1) == -1.5);
        TC_ASSERT_EXPR(m.at(2) == 2.5);
        TC_ASSERT_EXPR(m.at(3) == 3.5);
        TC_ASSERT_EXPR(m.at(4) == 4.5);
        TC_ASSERT_EXPR(m.at(5) == 5.5);
#ifndef TEST_HAS_NO_EXCEPTIONS
        try
        {
            TEST_IGNORE_NODISCARD m.at(6);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
        }
#endif
        TC_ASSERT_EXPR(m.at(7) == 7.5);
        TC_ASSERT_EXPR(m.at(8) == 8.5);
        TC_ASSERT_EXPR(m.size() == 7);
    }
    {
        typedef std::pair<const int, double> V;
        V ar[] =
        {
            V(1, 1.5),
            V(2, 2.5),
            V(3, 3.5),
            V(4, 4.5),
            V(5, 5.5),
            V(7, 7.5),
            V(8, 8.5),
        };
        const std::map<int, double> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 7);
        TC_ASSERT_EXPR(m.at(1) == 1.5);
        TC_ASSERT_EXPR(m.at(2) == 2.5);
        TC_ASSERT_EXPR(m.at(3) == 3.5);
        TC_ASSERT_EXPR(m.at(4) == 4.5);
        TC_ASSERT_EXPR(m.at(5) == 5.5);
#ifndef TEST_HAS_NO_EXCEPTIONS
        try
        {
            TEST_IGNORE_NODISCARD m.at(6);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
        }
#endif
        TC_ASSERT_EXPR(m.at(7) == 7.5);
        TC_ASSERT_EXPR(m.at(8) == 8.5);
        TC_ASSERT_EXPR(m.size() == 7);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
