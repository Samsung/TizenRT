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
// <list>

// void splice(const_iterator position, list& x, iterator first, iterator last);

#include <list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_ops_splice_pos_list_iter_iter(void) {
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5, 6};
    {
        std::list<int> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, std::next(l1.begin()), std::next(l1.begin()));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 3);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
    }
    {
        std::list<int> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, std::next(l1.begin()), std::next(l1.begin(), 2));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 3);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
    }
    {
        std::list<int> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, std::next(l1.begin()), std::next(l1.begin(), 3));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 3);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int> l2(a2, a2+3);
        l1.splice(l1.begin(), l2, std::next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 5);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int> l2(a2, a2+3);
        l1.splice(std::next(l1.begin()), l2, std::next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 5);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int> l2(a2, a2+3);
        l1.splice(l1.end(), l2, std::next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 5);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        ++i;
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
#if TEST_STD_VER >= 11
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, std::next(l1.begin()), std::next(l1.begin()));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 3);
        std::list<int, min_allocator<int>>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
    }
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, std::next(l1.begin()), std::next(l1.begin(), 2));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 3);
        std::list<int, min_allocator<int>>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
    }
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, std::next(l1.begin()), std::next(l1.begin(), 3));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 3);
        std::list<int, min_allocator<int>>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
    }
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        std::list<int, min_allocator<int>> l2(a2, a2+3);
        l1.splice(l1.begin(), l2, std::next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 5);
        std::list<int, min_allocator<int>>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        std::list<int, min_allocator<int>> l2(a2, a2+3);
        l1.splice(std::next(l1.begin()), l2, std::next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 5);
        std::list<int, min_allocator<int>>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        std::list<int, min_allocator<int>> l2(a2, a2+3);
        l1.splice(l1.end(), l2, std::next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(std::distance(l1.begin(), l1.end()) == 5);
        std::list<int, min_allocator<int>>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        ++i;
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
#endif

  return 0;
}
