//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// <map>

// class map

// node_type extract(const_iterator);

#include <map>
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

int tc_containers_associative_map_map_modifiers_extract_iterator(void) {
    {
        using map_type = std::map<int, int>;
        map_type m = {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}};
        test(m);
    }

    {
        std::map<Counter<int>, Counter<int>> m =
            {{1,1}, {2,2}, {3,3}, {4,4}, {5,5}, {6,6}};
        TC_ASSERT_EXPR(Counter_base::gConstructed == 12);
        test(m);
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }

    {
        using min_alloc_map =
            std::map<int, int, std::less<int>,
                     min_allocator<std::pair<const int, int>>>;
        min_alloc_map m = {{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}};
        test(m);
    }

  return 0;
}
