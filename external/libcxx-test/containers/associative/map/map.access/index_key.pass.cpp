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

// class map

// mapped_type& operator[](const key_type& k);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
#include "min_allocator.h"
#include "private_constructor.h"
#if TEST_STD_VER >= 11
#include "container_test_types.h"
#include "libcxx_tc_common.h"
#endif

int tc_containers_associative_map_map_access_index_key(void) {
    {
    typedef std::pair<const int, double> V;
    V ar[] =
    {
        V(1, 1.5),
        V(2, 2.5),
        V(3, 3.5),
        V(4, 4.5),
        V(5, 5.5),
        V(7, 7.5),
        V(8, 8.5),
    };
    std::map<int, double> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
    TC_ASSERT_EXPR(m.size() == 7);
    TC_ASSERT_EXPR(m[1] == 1.5);
    TC_ASSERT_EXPR(m.size() == 7);
    m[1] = -1.5;
    TC_ASSERT_EXPR(m[1] == -1.5);
    TC_ASSERT_EXPR(m.size() == 7);
    TC_ASSERT_EXPR(m[6] == 0);
    TC_ASSERT_EXPR(m.size() == 8);
    m[6] = 6.5;
    TC_ASSERT_EXPR(m[6] == 6.5);
    TC_ASSERT_EXPR(m.size() == 8);
    }
#if TEST_STD_VER >= 11
    {
    typedef std::pair<const int, double> V;
    V ar[] =
    {
        V(1, 1.5),
        V(2, 2.5),
        V(3, 3.5),
        V(4, 4.5),
        V(5, 5.5),
        V(7, 7.5),
        V(8, 8.5),
    };
    std::map<int, double, std::less<int>, min_allocator<V>> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
    TC_ASSERT_EXPR(m.size() == 7);
    TC_ASSERT_EXPR(m[1] == 1.5);
    TC_ASSERT_EXPR(m.size() == 7);
    const int i = 1;
    m[i] = -1.5;
    TC_ASSERT_EXPR(m[1] == -1.5);
    TC_ASSERT_EXPR(m.size() == 7);
    TC_ASSERT_EXPR(m[6] == 0);
    TC_ASSERT_EXPR(m.size() == 8);
    m[6] = 6.5;
    TC_ASSERT_EXPR(m[6] == 6.5);
    TC_ASSERT_EXPR(m.size() == 8);
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
            const Key k(1);
            cc->expect<std::piecewise_construct_t const&, std::tuple<Key const&>&&, std::tuple<>&&>();
            MappedType& mref = c[k];
            TC_ASSERT_EXPR(!cc->unchecked());
            {
                DisableAllocationGuard g;
                MappedType& mref2 = c[k];
                TC_ASSERT_EXPR(&mref == &mref2);
            }
        }
        {
            Container c;
            Key k(1);
            cc->expect<std::piecewise_construct_t const&, std::tuple<Key const&>&&, std::tuple<>&&>();
            MappedType& mref = c[k];
            TC_ASSERT_EXPR(!cc->unchecked());
            {
                DisableAllocationGuard g;
                MappedType& mref2 = c[k];
                TC_ASSERT_EXPR(&mref == &mref2);
            }
        }
    }
#endif
#if TEST_STD_VER > 11
    {
    typedef std::pair<const int, double> V;
    V ar[] =
    {
        V(1, 1.5),
        V(2, 2.5),
        V(3, 3.5),
        V(4, 4.5),
        V(5, 5.5),
        V(7, 7.5),
        V(8, 8.5),
    };
    std::map<int, double, std::less<>> m(ar, ar+sizeof(ar)/sizeof(ar[0]));

    TC_ASSERT_EXPR(m.size() == 7);
    TC_ASSERT_EXPR(m[1] == 1.5);
    TC_ASSERT_EXPR(m.size() == 7);
    m[1] = -1.5;
    TC_ASSERT_EXPR(m[1] == -1.5);
    TC_ASSERT_EXPR(m.size() == 7);
    TC_ASSERT_EXPR(m[6] == 0);
    TC_ASSERT_EXPR(m.size() == 8);
    m[6] = 6.5;
    TC_ASSERT_EXPR(m[6] == 6.5);
    TC_ASSERT_EXPR(m.size() == 8);
    }
#endif

  return 0;
}
