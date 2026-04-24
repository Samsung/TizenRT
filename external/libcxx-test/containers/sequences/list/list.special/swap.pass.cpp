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

// template <class T, class Alloc>
//   void swap(list<T,Alloc>& x, list<T,Alloc>& y);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_special_swap(void) {
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::list<int> c1(a1, a1 + 5);
        std::list<int> c2(a2, a2 + 7);
        std::list<int>::iterator it1 = c1.begin();
        std::list<int>::const_iterator it2 = c2.begin();
        swap(c1, c2);
        TC_ASSERT_EXPR(c1 == std::list<int>(a2, a2 + 7));
        TC_ASSERT_EXPR(c2 == std::list<int>(a1, a1 + 5));
        TC_ASSERT_EXPR(it1 == c2.begin()); // Iterators remain valid
        TC_ASSERT_EXPR(it2 == c1.begin()); // Iterators remain valid
    }
    {
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::list<int> c1;
        std::list<int> c2(a2, a2 + 7);
        swap(c1, c2);
        TC_ASSERT_EXPR(c1 == std::list<int>(a2, a2 + 7));
        TC_ASSERT_EXPR(c2.empty());
        TC_ASSERT_EXPR(std::distance(c2.begin(), c2.end()) == 0);
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        std::list<int> c1(a1, a1 + 5);
        std::list<int> c2;
        swap(c1, c2);
        TC_ASSERT_EXPR(c1.empty());
        TC_ASSERT_EXPR(std::distance(c1.begin(), c1.end()) == 0);
        TC_ASSERT_EXPR(c2 == std::list<int>(a1, a1 + 5));
    }
    {
        std::list<int> c1;
        std::list<int> c2;
        swap(c1, c2);
        TC_ASSERT_EXPR(c1.empty());
        TC_ASSERT_EXPR(std::distance(c1.begin(), c1.end()) == 0);
        TC_ASSERT_EXPR(c2.empty());
        TC_ASSERT_EXPR(std::distance(c2.begin(), c2.end()) == 0);
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        typedef test_allocator<int> A;
        std::list<int, A> c1(a1, a1 + 5, A(1));
        std::list<int, A> c2(a2, a2 + 7, A(1));
        swap(c1, c2);
        TC_ASSERT_EXPR((c1 == std::list<int, A>(a2, a2 + 7)));
        TC_ASSERT_EXPR(c1.get_allocator() == A(1));
        TC_ASSERT_EXPR((c2 == std::list<int, A>(a1, a1 + 5)));
        TC_ASSERT_EXPR(c2.get_allocator() == A(1));
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        typedef other_allocator<int> A;
        std::list<int, A> c1(a1, a1 + 5, A(1));
        std::list<int, A> c2(a2, a2 + 7, A(2));
        swap(c1, c2);
        TC_ASSERT_EXPR((c1 == std::list<int, A>(a2, a2 + 7)));
        TC_ASSERT_EXPR(c1.get_allocator() == A(2));
        TC_ASSERT_EXPR((c2 == std::list<int, A>(a1, a1 + 5)));
        TC_ASSERT_EXPR(c2.get_allocator() == A(1));
    }
#if TEST_STD_VER >= 11
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::list<int, min_allocator<int>> c1(a1, a1 + 5);
        std::list<int, min_allocator<int>> c2(a2, a2 + 7);
        swap(c1, c2);
        TC_ASSERT_EXPR((c1 == std::list<int, min_allocator<int>>(a2, a2 + 7)));
        TC_ASSERT_EXPR((c2 == std::list<int, min_allocator<int>>(a1, a1 + 5)));
    }
    {
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        std::list<int, min_allocator<int>> c1;
        std::list<int, min_allocator<int>> c2(a2, a2 + 7);
        swap(c1, c2);
        TC_ASSERT_EXPR((c1 == std::list<int, min_allocator<int>>(a2, a2 + 7)));
        TC_ASSERT_EXPR(c2.empty());
        TC_ASSERT_EXPR(std::distance(c2.begin(), c2.end()) == 0);
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        std::list<int, min_allocator<int>> c1(a1, a1 + 5);
        std::list<int, min_allocator<int>> c2;
        swap(c1, c2);
        TC_ASSERT_EXPR(c1.empty());
        TC_ASSERT_EXPR(std::distance(c1.begin(), c1.end()) == 0);
        TC_ASSERT_EXPR((c2 == std::list<int, min_allocator<int>>(a1, a1 + 5)));
    }
    {
        std::list<int, min_allocator<int>> c1;
        std::list<int, min_allocator<int>> c2;
        swap(c1, c2);
        TC_ASSERT_EXPR(c1.empty());
        TC_ASSERT_EXPR(std::distance(c1.begin(), c1.end()) == 0);
        TC_ASSERT_EXPR(c2.empty());
        TC_ASSERT_EXPR(std::distance(c2.begin(), c2.end()) == 0);
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        int a2[] = {0, 2, 4, 5, 6, 8, 11};
        typedef min_allocator<int> A;
        std::list<int, A> c1(a1, a1 + 5, A());
        std::list<int, A> c2(a2, a2 + 7, A());
        swap(c1, c2);
        TC_ASSERT_EXPR((c1 == std::list<int, A>(a2, a2 + 7)));
        TC_ASSERT_EXPR(c1.get_allocator() == A());
        TC_ASSERT_EXPR((c2 == std::list<int, A>(a1, a1 + 5)));
        TC_ASSERT_EXPR(c2.get_allocator() == A());
    }
#endif

    return 0;
}
