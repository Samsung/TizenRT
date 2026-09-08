//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// <set>

// class multiset

// node_type extract(key_type const&);

#include <set>
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
        TC_ASSERT_EXPR(t.value() == *copy);
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

int tc_containers_associative_multiset_extract_key(void) {
    {
        std::multiset<int> m = {1, 2, 3, 4, 5, 6};
        int keys[] = {1, 2, 3, 4, 5, 6};
        test(m, std::begin(keys), std::end(keys));
    }

    {
        std::multiset<Counter<int>> m = {1, 2, 3, 4, 5, 6};
        {
            Counter<int> keys[] = {1, 2, 3, 4, 5, 6};
            TC_ASSERT_EXPR(Counter_base::gConstructed == 6+6);
            test(m, std::begin(keys), std::end(keys));
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }

    {
        using min_alloc_set = std::multiset<int, std::less<int>, min_allocator<int>>;
        min_alloc_set m = {1, 2, 3, 4, 5, 6};
        int keys[] = {1, 2, 3, 4, 5, 6};
        test(m, std::begin(keys), std::end(keys));
    }

  return 0;
}
