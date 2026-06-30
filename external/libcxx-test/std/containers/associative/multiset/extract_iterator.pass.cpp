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

// node_type extract(const_iterator);

#include <set>
#include "test_macros.h"
#include "min_allocator.h"
#include "Counter.h"
#include "libcxx_tc_common.h"

template <class Container>
void test(Container& c)
{
    std::size_t sz = c.size();

    for (auto first = c.cbegin(); first != c.cend();)
    {
        auto key_value = *first;
        typename Container::node_type t = c.extract(first++);
        --sz;
        TC_ASSERT_EXPR(t.value() == key_value);
        TC_ASSERT_EXPR(t.get_allocator() == c.get_allocator());
        TC_ASSERT_EXPR(sz == c.size());
    }

    TC_ASSERT_EXPR(c.size() == 0);
}

int tc_containers_associative_multiset_extract_iterator(void) {
    {
        using set_type = std::multiset<int>;
        set_type m = {1, 2, 3, 4, 5, 6};
        test(m);
    }

    {
        std::multiset<Counter<int>> m = {1, 2, 3, 4, 5, 6};
        TC_ASSERT_EXPR(Counter_base::gConstructed == 6);
        test(m);
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }

    {
        using min_alloc_set = std::multiset<int, std::less<int>, min_allocator<int>>;
        min_alloc_set m = {1, 2, 3, 4, 5, 6};
        test(m);
    }

  return 0;
}
