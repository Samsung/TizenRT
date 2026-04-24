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

// class map

// insert_return_type insert(node_type&&);

#include <map>
#include <type_traits>
#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class Container>
typename Container::node_type
node_factory(typename Container::key_type const& key,
             typename Container::mapped_type const& mapped)
{
    static Container c;
    auto p = c.insert({key, mapped});
    TC_ASSERT_EXPR(p.second);
    return c.extract(p.first);
}

template <class Container>
void test(Container& c)
{
    auto* nf = &node_factory<Container>;

    for (int i = 0; i != 10; ++i)
    {
        typename Container::node_type node = nf(i, i + 1);
        TC_ASSERT_EXPR(!node.empty());
        typename Container::insert_return_type irt = c.insert(std::move(node));
        TC_ASSERT_EXPR(node.empty());
        TC_ASSERT_EXPR(irt.inserted);
        TC_ASSERT_EXPR(irt.node.empty());
        TC_ASSERT_EXPR(irt.position->first == i && irt.position->second == i + 1);
    }

    TC_ASSERT_EXPR(c.size() == 10);

    { // Insert empty node.
        typename Container::node_type def;
        auto irt = c.insert(std::move(def));
        TC_ASSERT_EXPR(def.empty());
        TC_ASSERT_EXPR(!irt.inserted);
        TC_ASSERT_EXPR(irt.node.empty());
        TC_ASSERT_EXPR(irt.position == c.end());
    }

    { // Insert duplicate node.
        typename Container::node_type dupl = nf(0, 42);
        auto irt = c.insert(std::move(dupl));
        TC_ASSERT_EXPR(dupl.empty());
        TC_ASSERT_EXPR(!irt.inserted);
        TC_ASSERT_EXPR(!irt.node.empty());
        TC_ASSERT_EXPR(irt.position == c.find(0));
        TC_ASSERT_EXPR(irt.node.key() == 0 && irt.node.mapped() == 42);
    }

    TC_ASSERT_EXPR(c.size() == 10);

    for (int i = 0; i != 10; ++i)
    {
        TC_ASSERT_EXPR(c.count(i) == 1);
        TC_ASSERT_EXPR(c[i] == i + 1);
    }
}

int tc_containers_associative_map_map_modifiers_insert_node_type(void) {
    std::map<int, int> m;
    test(m);
    std::map<int, int, std::less<int>, min_allocator<std::pair<const int, int>>> m2;
    test(m2);

  return 0;
}
