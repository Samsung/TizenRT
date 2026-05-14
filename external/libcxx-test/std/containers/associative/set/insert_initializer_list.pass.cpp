//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <set>

// class set

// void insert(initializer_list<value_type> il);

#include <set>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_insert_initializer_list(void) {
    {
    typedef std::set<int> C;
    typedef C::value_type V;
    C m = {10, 8};
    m.insert({1, 2, 3, 4, 5, 6});
    TC_ASSERT_EXPR(m.size() == 8);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.begin(), m.end())) == m.size());
    C::const_iterator i = m.cbegin();
    TC_ASSERT_EXPR(*i == V(1));
    TC_ASSERT_EXPR(*++i == V(2));
    TC_ASSERT_EXPR(*++i == V(3));
    TC_ASSERT_EXPR(*++i == V(4));
    TC_ASSERT_EXPR(*++i == V(5));
    TC_ASSERT_EXPR(*++i == V(6));
    TC_ASSERT_EXPR(*++i == V(8));
    TC_ASSERT_EXPR(*++i == V(10));
    }
    {
    typedef std::set<int, std::less<int>, min_allocator<int>> C;
    typedef C::value_type V;
    C m = {10, 8};
    m.insert({1, 2, 3, 4, 5, 6});
    TC_ASSERT_EXPR(m.size() == 8);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(m.begin(), m.end())) == m.size());
    C::const_iterator i = m.cbegin();
    TC_ASSERT_EXPR(*i == V(1));
    TC_ASSERT_EXPR(*++i == V(2));
    TC_ASSERT_EXPR(*++i == V(3));
    TC_ASSERT_EXPR(*++i == V(4));
    TC_ASSERT_EXPR(*++i == V(5));
    TC_ASSERT_EXPR(*++i == V(6));
    TC_ASSERT_EXPR(*++i == V(8));
    TC_ASSERT_EXPR(*++i == V(10));
    }

  return 0;
}
