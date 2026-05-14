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
//   pair<iterator, bool> emplace(Args&&... args);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "DefaultOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_emplace(void) {
    {
        typedef std::set<DefaultOnly> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        TC_ASSERT_EXPR(DefaultOnly::count == 0);
        R r = m.emplace();
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*m.begin() == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 1);

        r = m.emplace();
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*m.begin() == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 1);
    }
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    {
        typedef std::set<Emplaceable> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.emplace();
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*m.begin() == Emplaceable());
        r = m.emplace(2, 3.5);
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == std::next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r.first == Emplaceable(2, 3.5));
        r = m.emplace(2, 3.5);
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(r.first == std::next(m.begin()));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*r.first == Emplaceable(2, 3.5));
    }
    {
        typedef std::set<int> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.emplace(M::value_type(2));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r.first == 2);
    }
    {
        typedef std::set<int, std::less<int>, min_allocator<int>> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.emplace(M::value_type(2));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r.first == 2);
    }

  return 0;
}
