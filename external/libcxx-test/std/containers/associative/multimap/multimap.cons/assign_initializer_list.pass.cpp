/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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

// multimap& operator=(initializer_list<value_type> il);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"


int tc_libcxx_containers_multimap_cons_assign_initializer_list(void)
{
    {
    typedef std::multimap<int, double> C;
    typedef C::value_type V;
    C m = {{20, 1}};
    m =
           {
               {1, 1},
               {1, 1.5},
               {1, 2},
               {2, 1},
               {2, 1.5},
               {2, 2},
               {3, 1},
               {3, 1.5},
               {3, 2}
           };
    TC_ASSERT_EXPR(m.size() == 9);
    TC_ASSERT_EXPR(distance(m.begin(), m.end()) == 9);
    C::const_iterator i = m.cbegin();
    TC_ASSERT_EXPR(*i == V(1, 1));
    TC_ASSERT_EXPR(*++i == V(1, 1.5));
    TC_ASSERT_EXPR(*++i == V(1, 2));
    TC_ASSERT_EXPR(*++i == V(2, 1));
    TC_ASSERT_EXPR(*++i == V(2, 1.5));
    TC_ASSERT_EXPR(*++i == V(2, 2));
    TC_ASSERT_EXPR(*++i == V(3, 1));
    TC_ASSERT_EXPR(*++i == V(3, 1.5));
    TC_ASSERT_EXPR(*++i == V(3, 2));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
