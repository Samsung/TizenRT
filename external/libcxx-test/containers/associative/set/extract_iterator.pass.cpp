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

// <set>

// class set

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

int tc_containers_associative_set_extract_iterator(void) {
    {
        using set_type = std::set<int>;
        set_type m = {1, 2, 3, 4, 5, 6};
        test(m);
    }

    {
        std::set<Counter<int>> m = {1, 2, 3, 4, 5, 6};
        TC_ASSERT_EXPR(Counter_base::gConstructed == 6);
        test(m);
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }

    {
        using min_alloc_set = std::set<int, std::less<int>, min_allocator<int>>;
        min_alloc_set m = {1, 2, 3, 4, 5, 6};
        test(m);
    }

  return 0;
}
