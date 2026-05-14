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

// iterator erase(const_iterator first, const_iterator last);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_erase_iter_iter(void) {
    {
        typedef std::set<int> M;
        typedef int V;
        typedef M::iterator I;
        V ar[] =
        {
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 8);
        I i = m.erase(std::next(m.cbegin(), 5), std::next(m.cbegin(), 5));
        TC_ASSERT_EXPR(m.size() == 8);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 4);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 7) == 8);

        i = m.erase(std::next(m.cbegin(), 3), std::next(m.cbegin(), 4));
        TC_ASSERT_EXPR(m.size() == 7);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 8);

        i = m.erase(std::next(m.cbegin(), 2), std::next(m.cbegin(), 5));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 8);

        i = m.erase(std::next(m.cbegin(), 0), std::next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 8);

        i = m.erase(m.cbegin(), m.cend());
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == m.end());
    }
#if TEST_STD_VER >= 11
    {
        typedef std::set<int, std::less<int>, min_allocator<int>> M;
        typedef int V;
        typedef M::iterator I;
        V ar[] =
        {
            1,
            2,
            3,
            4,
            5,
            6,
            7,
            8
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 8);
        I i = m.erase(std::next(m.cbegin(), 5), std::next(m.cbegin(), 5));
        TC_ASSERT_EXPR(m.size() == 8);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 4);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 7) == 8);

        i = m.erase(std::next(m.cbegin(), 3), std::next(m.cbegin(), 4));
        TC_ASSERT_EXPR(m.size() == 7);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 5);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 6);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 8);

        i = m.erase(std::next(m.cbegin(), 2), std::next(m.cbegin(), 5));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 8);

        i = m.erase(std::next(m.cbegin(), 0), std::next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(i == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 7);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 8);

        i = m.erase(m.cbegin(), m.cend());
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == m.end());
    }
#endif

  return 0;
}
