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

template <typename MMap>
struct CallCompMember : MMap::value_compare {
    CallCompMember(const typename MMap::value_compare& vc) : MMap::value_compare(vc) {}

    typedef typename MMap::value_type value_type;
    bool operator()(const value_type& value1, const value_type& value2) const {
        return this->comp(value1.first, value2.first);
    }
};

int tc_containers_associative_multimap_multimap_value_compare_invoke(void) {
    typedef std::multimap<int, std::string> map_type;

    map_type m;
    map_type::iterator i1 = m.insert(map_type::value_type(1, "abc"));
    map_type::iterator i2 = m.insert(map_type::value_type(2, "abc"));

    const map_type::value_compare vc = m.value_comp();
    CallCompMember<map_type> call_comp = m.value_comp();

    TC_ASSERT_EXPR(vc(*i1, *i2));
    TC_ASSERT_EXPR(call_comp(*i1, *i2));

    TC_ASSERT_EXPR(!vc(*i2, *i1));
    TC_ASSERT_EXPR(!call_comp(*i2, *i1));

    return 0;
}
