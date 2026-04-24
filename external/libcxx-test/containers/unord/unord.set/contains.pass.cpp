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
// UNSUPPORTED: c++03, c++11, c++14, c++17

#include <cassert>
#include <unordered_set>
#include "libcxx_tc_common.h"

// <unordered_set>

// bool contains(const key_type& x) const;

template <typename T, typename V, typename B, typename... Vals>
void test(B bad, Vals... args) {
    T set;
    V vals[] = {args...};

    for (auto& v : vals) set.insert(v);
    for (auto& v : vals) TC_ASSERT_EXPR(set.contains(v));

    TC_ASSERT_EXPR(!set.contains(bad));
}

struct E { int a = 1; double b = 1; char c = 1; };

int tc_containers_unord_unord_set_contains(void) {
    {
        test<std::unordered_set<int>, int>(14, 10, 11, 12, 13);
        test<std::unordered_set<char>, char>('e', 'a', 'b', 'c', 'd');
    }
    {
        test<std::unordered_multiset<int>, int>(14, 10, 11, 12, 13);
        test<std::unordered_multiset<char>, char>('e', 'a', 'b', 'c', 'd');
    }

    return 0;
}
