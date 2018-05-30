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

// <map>

// class multimap

// multimap(const multimap& m, const allocator_type& a);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_compare.h"
#include "test_allocator.h"

int tc_libcxx_containers_multimap_cons_copy_alloc(void)
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
    typedef test_allocator<V> A;
    std::multimap<int, double, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(7));
    std::multimap<int, double, C, A> m(mo, A(3));
    TC_ASSERT_EXPR(m == mo);
    TC_ASSERT_EXPR(m.get_allocator() == A(3));
    TC_ASSERT_EXPR(m.key_comp() == C(5));

    TC_ASSERT_EXPR(mo.get_allocator() == A(7));
    TC_ASSERT_EXPR(mo.key_comp() == C(5));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
