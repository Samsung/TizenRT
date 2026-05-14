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

// template <class... Args>
//   iterator emplace(Args&&... args);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "DefaultOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_emplace(void) {
    {
        typedef std::multiset<DefaultOnly> M;
        typedef M::iterator R;
        M m;
        TC_ASSERT_EXPR(DefaultOnly::count == 0);
        R r = m.emplace();
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*m.begin() == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 1);

        r = m.emplace();
        TC_ASSERT_EXPR(r == std::next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*m.begin() == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 2);
    }
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    {
        typedef std::multiset<Emplaceable> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace();
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*m.begin() == Emplaceable());
        r = m.emplace(2, 3.5);
        TC_ASSERT_EXPR(r == std::next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r == Emplaceable(2, 3.5));
        r = m.emplace(2, 3.5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r == Emplaceable(2, 3.5));
    }
    {
        typedef std::multiset<int> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace(M::value_type(2));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r == 2);
    }
    {
        typedef std::multiset<int, std::less<int>, min_allocator<int>> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace(M::value_type(2));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r == 2);
    }

  return 0;
}
