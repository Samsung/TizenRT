//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
