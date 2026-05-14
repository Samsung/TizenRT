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
#include <map>
#include "libcxx_tc_common.h"

// <map>

// bool contains(const key_type& x) const;

template <typename T, typename P, typename B, typename... Pairs>
void test(B bad, Pairs... args) {
    T map;
    P pairs[] = {args...};

    for (auto& p : pairs) map.insert(p);
    for (auto& p : pairs) TC_ASSERT_EXPR(map.contains(p.first));

    TC_ASSERT_EXPR(!map.contains(bad));
}

struct E { int a = 1; double b = 1; char c = 1; };

int tc_containers_associative_map_map_ops_contains(void) {
    {
        test<std::map<char, int>, std::pair<char, int> >(
            'e', std::make_pair('a', 10), std::make_pair('b', 11),
            std::make_pair('c', 12), std::make_pair('d', 13));

        test<std::map<char, char>, std::pair<char, char> >(
            'e', std::make_pair('a', 'a'), std::make_pair('b', 'a'),
            std::make_pair('c', 'a'), std::make_pair('d', 'b'));

        test<std::map<int, E>, std::pair<int, E> >(
            -1, std::make_pair(1, E{}), std::make_pair(2, E{}),
            std::make_pair(3, E{}), std::make_pair(4, E{}));
    }
    {
        test<std::multimap<char, int>, std::pair<char, int> >(
            'e', std::make_pair('a', 10), std::make_pair('b', 11),
            std::make_pair('c', 12), std::make_pair('d', 13));

        test<std::multimap<char, char>, std::pair<char, char> >(
            'e', std::make_pair('a', 'a'), std::make_pair('b', 'a'),
            std::make_pair('c', 'a'), std::make_pair('d', 'b'));

        test<std::multimap<int, E>, std::pair<int, E> >(
            -1, std::make_pair(1, E{}), std::make_pair(2, E{}),
            std::make_pair(3, E{}), std::make_pair(4, E{}));
    }

    return 0;
}
