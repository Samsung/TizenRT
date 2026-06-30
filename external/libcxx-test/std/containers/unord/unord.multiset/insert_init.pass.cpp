//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// void insert(initializer_list<value_type> il);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multiset_insert_init(void) {
    {
        typedef std::unordered_multiset<int> C;
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
        TC_ASSERT_EXPR(c.size() == 6);
        TC_ASSERT_EXPR(c.count(1) == 2);
        TC_ASSERT_EXPR(c.count(2) == 2);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
    }
    {
        typedef std::unordered_multiset<int, std::hash<int>,
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
        TC_ASSERT_EXPR(c.size() == 6);
        TC_ASSERT_EXPR(c.count(1) == 2);
        TC_ASSERT_EXPR(c.count(2) == 2);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);
    }

  return 0;
}
