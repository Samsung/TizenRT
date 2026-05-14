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

// class value_compare

// bool operator()( const value_type& lhs, const value_type& rhs ) const;

#include <map>
#include <cassert>
#include <string>
#include <utility>
#include "libcxx_tc_common.h"

template <typename Map>
struct CallCompMember : Map::value_compare {
    CallCompMember(const typename Map::value_compare& vc) : Map::value_compare(vc) {}

    typedef typename Map::value_type value_type;
    bool operator()(const value_type& value1, const value_type& value2) const {
        return this->comp(value1.first, value2.first);
    }
};

int tc_containers_associative_map_map_value_compare_invoke(void) {
    typedef std::map<int, std::string> map_type;

    map_type m;
    std::pair<map_type::iterator, bool> p1 = m.insert(map_type::value_type(1, "abc"));
    std::pair<map_type::iterator, bool> p2 = m.insert(map_type::value_type(2, "abc"));

    const map_type::value_compare vc = m.value_comp();
    CallCompMember<map_type> call_comp = m.value_comp();

    TC_ASSERT_EXPR(vc(*p1.first, *p2.first));
    TC_ASSERT_EXPR(call_comp(*p1.first, *p2.first));

    TC_ASSERT_EXPR(!vc(*p2.first, *p1.first));
    TC_ASSERT_EXPR(!call_comp(*p2.first, *p1.first));

    return 0;
}
