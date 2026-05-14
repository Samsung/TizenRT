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
// UNSUPPORTED: c++03

// <set>

// class multiset

// multiset(initializer_list<value_type> il, const key_compare& comp = key_compare());

#include <set>
#include <cassert>
#include "test_macros.h"
#include "../../../test_compare.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_initializer_list_compare(void) {
    typedef test_less<int> Cmp;
    typedef std::multiset<int, Cmp> C;
    typedef C::value_type V;
    C m({1, 2, 3, 4, 5, 6}, Cmp(10));
    TC_ASSERT_EXPR(m.size() == 6);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 6);
    C::const_iterator i = m.cbegin();
    TC_ASSERT_EXPR(*i == V(1));
    TC_ASSERT_EXPR(*++i == V(2));
    TC_ASSERT_EXPR(*++i == V(3));
    TC_ASSERT_EXPR(*++i == V(4));
    TC_ASSERT_EXPR(*++i == V(5));
    TC_ASSERT_EXPR(*++i == V(6));
    TC_ASSERT_EXPR(m.key_comp() == Cmp(10));

  return 0;
}
