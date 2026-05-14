//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <map>

// class map

// void insert(initializer_list<value_type> il);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_modifiers_insert_initializer_list(void) {
    {
    typedef std::pair<const int, double> V;
    std::map<int, double> m =
                            {
                                {1, 1},
                                {1, 1.5},
                                {1, 2},
                                {3, 1},
                                {3, 1.5},
                                {3, 2}
                            };
    m.insert({
                 {2, 1},
                 {2, 1.5},
                 {2, 2},
             });
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*std::next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*std::next(m.begin(), 2) == V(3, 1));
    }
    {
    typedef std::pair<const int, double> V;
    std::map<int, double, std::less<int>, min_allocator<V>> m =
                            {
                                {1, 1},
                                {1, 1.5},
                                {1, 2},
                                {3, 1},
                                {3, 1.5},
                                {3, 2}
                            };
    m.insert({
                 {2, 1},
                 {2, 1.5},
                 {2, 2},
             });
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 3);
    TC_ASSERT_EXPR(*m.begin() == V(1, 1));
    TC_ASSERT_EXPR(*std::next(m.begin()) == V(2, 1));
    TC_ASSERT_EXPR(*std::next(m.begin(), 2) == V(3, 1));
    }

  return 0;
}
