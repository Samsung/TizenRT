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
// <map>

// key_compare key_comp() const;

#include <map>
#include <cassert>
#include <string>
#include "libcxx_tc_common.h"

int tc_containers_associative_multimap_multimap_observers_key_comp(void) {
    typedef std::multimap<int, std::string> map_type;

    map_type m;
    map_type::iterator i1 = m.insert(map_type::value_type(1, "abc"));
    map_type::iterator i2 = m.insert(map_type::value_type(2, "abc"));

    const map_type& cm = m;

    TC_ASSERT_EXPR(cm.key_comp()(i1->first, i2->first));
    TC_ASSERT_EXPR(!cm.key_comp()(i2->first, i1->first));

    return 0;
}
