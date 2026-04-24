/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
// UNSUPPORTED: c++03

// <set>

// class multiset

// template <class... Args>
//   iterator emplace_hint(const_iterator position, Args&&... args);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "DefaultOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_emplace_hint(void) {
    {
        typedef std::multiset<DefaultOnly> M;
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
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(*m.begin() == DefaultOnly());
        TC_ASSERT_EXPR(DefaultOnly::count == 2);
    }
    TC_ASSERT_EXPR(DefaultOnly::count == 0);
    {
        typedef std::multiset<Emplaceable> M;
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
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r == Emplaceable(2, 3.5));
    }
    {
        typedef std::multiset<int> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.cend(), M::value_type(2));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r == 2);
    }
    {
        typedef std::multiset<int, std::less<int>, min_allocator<int>> M;
        typedef M::iterator R;
        M m;
        R r = m.emplace_hint(m.cend(), M::value_type(2));
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(*r == 2);
    }

  return 0;
}
