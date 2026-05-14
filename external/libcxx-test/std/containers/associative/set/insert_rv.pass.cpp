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

// pair<iterator, bool> insert(value_type&& v);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_insert_rv(void) {
    {
        typedef std::set<MoveOnly> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.insert(M::value_type(2));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r.first == 2);

        r = m.insert(M::value_type(1));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r.first == 1);

        r = m.insert(M::value_type(3));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r.first == 3);

        r = m.insert(M::value_type(3));
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(r.first == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r.first == 3);
    }
    {
        typedef std::set<MoveOnly, std::less<MoveOnly>, min_allocator<MoveOnly>> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.insert(M::value_type(2));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r.first == 2);

        r = m.insert(M::value_type(1));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r.first == 1);

        r = m.insert(M::value_type(3));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r.first == 3);

        r = m.insert(M::value_type(3));
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(r.first == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r.first == 3);
    }

  return 0;
}
