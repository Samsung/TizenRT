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

// <unordered_map>

// class unordered_multimap

// node_type extract(const_iterator);

#include <unordered_map>
#include "test_macros.h"
#include "min_allocator.h"
#include "Counter.h"
#include "libcxx_tc_common.h"

template <class Container>
void test(Container& c)
{
    std::size_t sz = c.size();

    auto some_key = c.cbegin()->first;

    for (auto first = c.cbegin(); first != c.cend();)
    {
        auto key_value = first->first;
        typename Container::node_type t = c.extract(first++);
        --sz;
        TC_ASSERT_EXPR(t.key() == key_value);
        t.key() = some_key;
        TC_ASSERT_EXPR(t.key() == some_key);
        TC_ASSERT_EXPR(t.get_allocator() == c.get_allocator());
        TC_ASSERT_EXPR(sz == c.size());
    }

    TC_ASSERT_EXPR(c.size() == 0);
}

int tc_containers_unord_unord_multimap_unord_multimap_modifiers_extract_iterator(void) {
    {
        using map_type = std::unordered_multimap<int, int>;
        map_type m = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}};
        test(m);
    }

    {
        std::unordered_multimap<Counter<int>, Counter<int>> m =
            {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}};
        TC_ASSERT_EXPR(Counter_base::gConstructed == 12);
        test(m);
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }

    {
        using min_alloc_map =
            std::unordered_multimap<int, int, std::hash<int>, std::equal_to<int>,
                                    min_allocator<std::pair<const int, int>>>;
        min_alloc_map m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
        test(m);
    }

  return 0;
}
