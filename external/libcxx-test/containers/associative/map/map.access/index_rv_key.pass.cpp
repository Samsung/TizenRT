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

// <map>

// class map

// mapped_type& operator[](key_type&& k);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "container_test_types.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_access_index_rv_key(void) {
    {
    std::map<MoveOnly, double> m;
    TC_ASSERT_EXPR(m.size() == 0);
    TC_ASSERT_EXPR(m[1] == 0.0);
    TC_ASSERT_EXPR(m.size() == 1);
    m[1] = -1.5;
    TC_ASSERT_EXPR(m[1] == -1.5);
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(m[6] == 0);
    TC_ASSERT_EXPR(m.size() == 2);
    m[6] = 6.5;
    TC_ASSERT_EXPR(m[6] == 6.5);
    TC_ASSERT_EXPR(m.size() == 2);
    }
    {
    typedef std::pair<const MoveOnly, double> V;
    std::map<MoveOnly, double, std::less<MoveOnly>, min_allocator<V>> m;
    TC_ASSERT_EXPR(m.size() == 0);
    TC_ASSERT_EXPR(m[1] == 0.0);
    TC_ASSERT_EXPR(m.size() == 1);
    m[1] = -1.5;
    TC_ASSERT_EXPR(m[1] == -1.5);
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(m[6] == 0);
    TC_ASSERT_EXPR(m.size() == 2);
    m[6] = 6.5;
    TC_ASSERT_EXPR(m[6] == 6.5);
    TC_ASSERT_EXPR(m.size() == 2);
    }
    {
        // Use "container_test_types.h" to check what arguments get passed
        // to the allocator for operator[]
        using Container = TCT::map<>;
        using Key = Container::key_type;
        using MappedType = Container::mapped_type;
        ConstructController* cc = getConstructController();
        cc->reset();
        {
            Container c;
            Key k(1);
            cc->expect<std::piecewise_construct_t const&, std::tuple<Key &&>&&, std::tuple<>&&>();
            MappedType& mref = c[std::move(k)];
            TC_ASSERT_EXPR(!cc->unchecked());
            {
                Key k2(1);
                DisableAllocationGuard g;
                MappedType& mref2 = c[std::move(k2)];
                TC_ASSERT_EXPR(&mref == &mref2);
            }
        }
    }

  return 0;
}
