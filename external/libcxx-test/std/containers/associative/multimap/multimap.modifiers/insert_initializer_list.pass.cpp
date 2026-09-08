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

// class multimap

// static int insert(initializer_list<value_type> il);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_containers_multimap_modifiers_insert_initializer_list(void)
{
    {
    typedef std::multimap<int, double> C;
    typedef C::value_type V;
    C m =
           {
               {1, 1},
               {1, 2},
               {2, 1},
               {2, 2},
               {3, 1},
               {3, 2}
           };
    m.insert(
               {
                   {1, 1.5},
                   {2, 1.5},
                   {3, 1.5},
               }
            );
    TC_ASSERT_EXPR(m.size() == 9);
    TC_ASSERT_EXPR(distance(m.begin(), m.end()) == 9);
    C::const_iterator i = m.cbegin();
    TC_ASSERT_EXPR(*i == V(1, 1));
    TC_ASSERT_EXPR(*++i == V(1, 2));
    TC_ASSERT_EXPR(*++i == V(1, 1.5));
    TC_ASSERT_EXPR(*++i == V(2, 1));
    TC_ASSERT_EXPR(*++i == V(2, 2));
    TC_ASSERT_EXPR(*++i == V(2, 1.5));
    TC_ASSERT_EXPR(*++i == V(3, 1));
    TC_ASSERT_EXPR(*++i == V(3, 2));
    TC_ASSERT_EXPR(*++i == V(3, 1.5));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
