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
// <set>

// class set

// iterator insert(const_iterator position, const value_type& v);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_insert_iter_cv(void) {
    {
        typedef std::set<int> M;
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
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r == 3);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::set<int, std::less<int>, min_allocator<int>> M;
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
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(*r == 3);
    }
#endif

  return 0;
}
