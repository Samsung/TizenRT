//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <set>

// class multiset

// iterator insert(const_iterator position, value_type&& v);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_insert_iter_rv(void) {
    {
        typedef std::multiset<MoveOnly> M;
        typedef M::iterator R;
        M m;
        R r = m.insert(m.cend(), M::value_type(2));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r == 2);

        r = m.insert(m.cend(), M::value_type(1));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r == 1);

        r = m.insert(m.cend(), M::value_type(3));
        TC_ASSERT_EXPR(r == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r == 3);

        r = m.insert(m.cend(), M::value_type(3));
        TC_ASSERT_EXPR(r == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(*r == 3);
    }
    {
        typedef std::multiset<MoveOnly, std::less<MoveOnly>, min_allocator<MoveOnly>> M;
        typedef M::iterator R;
        M m;
        R r = m.insert(m.cend(), M::value_type(2));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r == 2);

        r = m.insert(m.cend(), M::value_type(1));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r == 1);

        r = m.insert(m.cend(), M::value_type(3));
        TC_ASSERT_EXPR(r == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r == 3);

        r = m.insert(m.cend(), M::value_type(3));
        TC_ASSERT_EXPR(r == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(*r == 3);
    }

  return 0;
}
