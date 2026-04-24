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

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// void insert(initializer_list<value_type> il);

#include <unordered_map>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_map_unord_map_modifiers_insert_init(void) {
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        C c;
        c.insert(
                    {
                        P(1, "one"),
                        P(2, "two"),
                        P(3, "three"),
                        P(4, "four"),
                        P(1, "four"),
                        P(2, "four"),
                    }
                );
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(2) == "two");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");
    }
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef std::pair<int, std::string> P;
        C c;
        c.insert(
                    {
                        P(1, "one"),
                        P(2, "two"),
                        P(3, "three"),
                        P(4, "four"),
                        P(1, "four"),
                        P(2, "four"),
                    }
                );
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(2) == "two");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");
    }

  return 0;
}
