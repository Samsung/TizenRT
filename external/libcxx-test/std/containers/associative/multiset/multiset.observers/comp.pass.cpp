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
// <set>

// key_compare key_comp() const;
// value_compare value_comp() const;

#include <set>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_observers_comp(void) {
    typedef std::multiset<int> set_type;

    set_type s;
    set_type::iterator i1 = s.insert(1);
    set_type::iterator i2 = s.insert(2);

    const set_type& cs = s;

    TC_ASSERT_EXPR(cs.key_comp()(*i1, *i2));
    TC_ASSERT_EXPR(!cs.key_comp()(*i2, *i1));

    TC_ASSERT_EXPR(cs.value_comp()(*i1, *i2));
    TC_ASSERT_EXPR(!cs.value_comp()(*i2, *i1));

    return 0;
}
