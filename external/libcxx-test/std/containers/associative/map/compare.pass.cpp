//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <map>

// template <class Key, class T, class Compare = less<Key>,
//           class Allocator = allocator<pair<const Key, T>>>
// class map

// https://llvm.org/PR16538
// https://llvm.org/PR16549

#include <map>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Key {
  template <typename T> Key(const T&) {}
  bool operator< (const Key&) const { return false; }
};

int tc_containers_associative_map_compare(void) {
    typedef std::map<Key, int> MapT;
    typedef MapT::iterator Iter;
    typedef std::pair<Iter, bool> IterBool;
    {
        MapT m_empty;
        MapT m_contains;
        m_contains[Key(0)] = 42;

        Iter it = m_empty.find(Key(0));
        TC_ASSERT_EXPR(it == m_empty.end());
        it = m_contains.find(Key(0));
        TC_ASSERT_EXPR(it != m_contains.end());
    }
    {
        MapT map;
        IterBool result = map.insert(std::make_pair(Key(0), 42));
        TC_ASSERT_EXPR(result.second);
        TC_ASSERT_EXPR(result.first->second == 42);
        IterBool result2 = map.insert(std::make_pair(Key(0), 43));
        TC_ASSERT_EXPR(!result2.second);
        TC_ASSERT_EXPR(map[Key(0)] == 42);
    }

  return 0;
}
