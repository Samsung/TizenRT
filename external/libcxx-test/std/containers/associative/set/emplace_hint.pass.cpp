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

// template <class... Args>
//   iterator emplace_hint(const_iterator position, Args&&... args);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "Emplaceable.h"
#include "DefaultOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_emplace_hint(void) {
    {
        typedef std::set<DefaultOnly> M;
        typedef M::iterator R;
        M m;
        TC_ASSERT_EXPR(DefaultOnly::count == 0);
        R r = m.emplace_hint(m.cend());
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*m.begin() == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 1);

        r = m.emplace_hint(m.cbegin());
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*m.begin() == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 1);
    }
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    {
        typedef std::set<Emplaceable> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.cend());
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*m.begin() == Emplaceable());
        r = m.emplace_hint(m.cend(), 2, 3.5);
        TC_ASSERT_EXPR(r == std::next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r == Emplaceable(2, 3.5));
        r = m.emplace_hint(m.cbegin(), 2, 3.5);
        TC_ASSERT_EXPR(r == std::next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r == Emplaceable(2, 3.5));
    }
    {
        typedef std::set<int> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.cend(), M::value_type(2));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r == 2);
    }
    {
        typedef std::set<int, std::less<int>, min_allocator<int>> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.cend(), M::value_type(2));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r == 2);
    }

  return 0;
}
