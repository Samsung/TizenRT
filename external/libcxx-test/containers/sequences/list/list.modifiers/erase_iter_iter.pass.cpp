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

// iterator erase(const_iterator first, const_iterator last);

#include <list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_modifiers_erase_iter_iter(void) {
    int a1[] = {1, 2, 3};
    {
        std::list<int> l1(a1, a1+3);
        std::list<int>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 3);
        TC_ASSERT_EXPR(i == l1.begin());
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin()));
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 2);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(l1 == std::list<int>(a1+1, a1+3));
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 2));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 1);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(l1 == std::list<int>(a1+2, a1+3));
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 3));
        TC_ASSERT_EXPR(l1.size() == 0);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 0);
        TC_ASSERT_EXPR(i == l1.begin());
    }
#if TEST_STD_VER >= 11
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        std::list<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 3);
        TC_ASSERT_EXPR(i == l1.begin());
    }
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        std::list<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin()));
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 2);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR((l1 == std::list<int, min_allocator<int>>(a1+1, a1+3)));
    }
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        std::list<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 2));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 1);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR((l1 == std::list<int, min_allocator<int>>(a1+2, a1+3)));
    }
    {
        std::list<int, min_allocator<int>> l1(a1, a1+3);
        std::list<int, min_allocator<int>>::iterator i = l1.erase(l1.cbegin(), std::next(l1.cbegin(), 3));
        TC_ASSERT_EXPR(l1.size() == 0);
        TC_ASSERT_EXPR(std::distance(l1.cbegin(), l1.cend()) == 0);
        TC_ASSERT_EXPR(i == l1.begin());
    }
#endif

  return 0;
}
