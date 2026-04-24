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

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// void insert(initializer_list<value_type> il);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_set_insert_init(void) {
    {
        typedef std::unordered_set<int> C;
        typedef int P;
        C c;
        c.insert(
                    {
                        P(1),
                        P(2),
                        P(3),
                        P(4),
                        P(1),
                        P(2)
                    }
                );
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
    }
    {
        typedef std::unordered_set<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        typedef int P;
        C c;
        c.insert(
                    {
                        P(1),
                        P(2),
                        P(3),
                        P(4),
                        P(1),
                        P(2)
                    }
                );
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
    }

  return 0;
}
