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

// mapped_type&       at(const key_type& k);
// const mapped_type& at(const key_type& k) const;

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "MoveOnly.h"
#include "test_macros.h"

int tc_libcxx_containers_unord_map_elem_at(void)
{
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.size() == 4);
        c.at(1) = "ONE";
        TC_ASSERT_EXPR(c.at(1) == "ONE");
#ifndef TEST_HAS_NO_EXCEPTIONS
        try
        {
            c.at(11) = "eleven";
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
        }
        TC_ASSERT_EXPR(c.size() == 4);
#endif
    }
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        const C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.at(1) == "one");
#ifndef TEST_HAS_NO_EXCEPTIONS
        try
        {
            TEST_IGNORE_NODISCARD c.at(11);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
        }
        TC_ASSERT_EXPR(c.size() == 4);
#endif
    }
    TC_SUCCESS_RESULT();
    return 0;
}
