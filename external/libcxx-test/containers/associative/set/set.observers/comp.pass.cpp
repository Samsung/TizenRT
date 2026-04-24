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

int tc_containers_associative_set_set_observers_comp(void) {
    typedef std::set<int> set_type;

    set_type s;
    std::pair<set_type::iterator, bool> p1 = s.insert(1);
    std::pair<set_type::iterator, bool> p2 = s.insert(2);

    const set_type& cs = s;

    TC_ASSERT_EXPR(cs.key_comp()(*p1.first, *p2.first));
    TC_ASSERT_EXPR(!cs.key_comp()(*p2.first, *p1.first));

    TC_ASSERT_EXPR(cs.value_comp()(*p1.first, *p2.first));
    TC_ASSERT_EXPR(!cs.value_comp()(*p2.first, *p1.first));

    return 0;
}
