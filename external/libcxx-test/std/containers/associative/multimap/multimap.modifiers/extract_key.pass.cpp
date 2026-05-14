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
// UNSUPPORTED: c++03, c++11, c++14

// <map>

// class multimap

// node_type extract(key_type const&);

#include <map>
#include "test_macros.h"
#include "min_allocator.h"
#include "Counter.h"
#include "libcxx_tc_common.h"

template <class Container, class KeyTypeIter>
void test(Container& c, KeyTypeIter first, KeyTypeIter last)
{
    std::size_t sz = c.size();
    TC_ASSERT_EXPR((std::size_t)std::distance(first, last) == sz);

    for (KeyTypeIter copy = first; copy != last; ++copy)
    {
        typename Container::node_type t = c.extract(*copy);
        TC_ASSERT_EXPR(!t.empty());
        --sz;
        TC_ASSERT_EXPR(t.key() == *copy);
        t.key() = *first; // We should be able to mutate key.
        TC_ASSERT_EXPR(t.key() == *first);
        TC_ASSERT_EXPR(t.get_allocator() == c.get_allocator());
        TC_ASSERT_EXPR(sz == c.size());
    }

    TC_ASSERT_EXPR(c.size() == 0);

    for (KeyTypeIter copy = first; copy != last; ++copy)
    {
        typename Container::node_type t = c.extract(*copy);
        TC_ASSERT_EXPR(t.empty());
    }
}

int tc_containers_associative_multimap_multimap_modifiers_extract_key(void) {
    {
        std::multimap<int, int> m = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}};
        int keys[] = {1, 2, 3, 4, 5, 6};
        test(m, std::begin(keys), std::end(keys));
    }

    {
        std::multimap<Counter<int>, Counter<int>> m =
            {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}};
        {
            Counter<int> keys[] = {1, 2, 3, 4, 5, 6};
            TC_ASSERT_EXPR(Counter_base::gConstructed == 12+6);
            test(m, std::begin(keys), std::end(keys));
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }

    {
        using min_alloc_map =
            std::multimap<int, int, std::less<int>,
                          min_allocator<std::pair<const int, int>>>;
        min_alloc_map m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
        int keys[] = {1, 2, 3, 4, 5, 6};
        test(m, std::begin(keys), std::end(keys));
    }

  return 0;
}
