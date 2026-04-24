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
// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// https://llvm.org/PR16538
// https://llvm.org/PR16549

#include <unordered_map>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Key {
  template <typename T> Key(const T&) {}
  bool operator== (const Key&) const { return true; }
};

namespace std
{
    template <>
    struct hash<Key>
    {
        std::size_t operator()(Key const &) const {return 0;}
    };
}

int tc_containers_unord_unord_map_compare(void) {
    typedef std::unordered_map<Key, int> MapT;
    typedef MapT::iterator Iter;
    MapT map;
    Iter it = map.find(Key(0));
    TC_ASSERT_EXPR(it == map.end());
    std::pair<Iter, bool> result = map.insert(std::make_pair(Key(0), 42));
    TC_ASSERT_EXPR(result.second);
    TC_ASSERT_EXPR(result.first->second == 42);

  return 0;
}
