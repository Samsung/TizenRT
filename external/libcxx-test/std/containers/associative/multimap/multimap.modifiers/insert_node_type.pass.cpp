//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// <map>

// class multimap

// iterator insert(node_type&&);

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
    auto it = c.insert({key, mapped});
    return c.extract(it);
}

template <class Container>
void test(Container& c)
{
    auto* nf = &node_factory<Container>;

    for (int i = 0; i != 10; ++i)
    {
        typename Container::node_type node = nf(i, i + 1);
        TC_ASSERT_EXPR(!node.empty());
        typename Container::iterator it = c.insert(std::move(node));
        TC_ASSERT_EXPR(node.empty());
        TC_ASSERT_EXPR(it == c.find(i) && it != c.end());
    }

    TC_ASSERT_EXPR(c.size() == 10);

    { // Insert empty node.
        typename Container::node_type def;
        auto it = c.insert(std::move(def));
        TC_ASSERT_EXPR(def.empty());
        TC_ASSERT_EXPR(it == c.end());
    }

    { // Insert duplicate node.
        typename Container::node_type dupl = nf(0, 42);
        auto it = c.insert(std::move(dupl));
        TC_ASSERT_EXPR(dupl.empty());
        TC_ASSERT_EXPR(it != c.end());
        TC_ASSERT_EXPR(it->second == 42);
    }

    TC_ASSERT_EXPR(c.size() == 11);
    TC_ASSERT_EXPR(c.count(0) == 2);
    for (int i = 1; i != 10; ++i)
    {
        TC_ASSERT_EXPR(c.count(i) == 1);
        TC_ASSERT_EXPR(c.find(i)->second == i + 1);
    }
}

int tc_containers_associative_multimap_multimap_modifiers_insert_node_type(void) {
    std::multimap<int, int> m;
    test(m);
    std::multimap<int, int, std::less<int>, min_allocator<std::pair<const int, int>>> m2;
    test(m2);

  return 0;
}
