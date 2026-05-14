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

// template< class T, class Alloc >
// bool operator==( const std::list<T,Alloc>& lhs,
//                  const std::list<T,Alloc>& rhs );

// template< class T, class Alloc >
// bool operator!=( const std::list<T,Alloc>& lhs,
//                  const std::list<T,Alloc>& rhs );

// template< class T, class Alloc >
// bool operator<( const std::list<T,Alloc>& lhs,
//                 const std::list<T,Alloc>& rhs );

// template< class T, class Alloc >
// bool operator<=( const std::list<T,Alloc>& lhs,
//                  const std::list<T,Alloc>& rhs );

// template< class T, class Alloc >
// bool operator>( const std::list<T,Alloc>& lhs,
//                 const std::list<T,Alloc>& rhs );

// template< class T, class Alloc >
// bool operator>=( const std::list<T,Alloc>& lhs,
//                  const std::list<T,Alloc>& rhs );

#include <list>
#include <cassert>

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_compare(void) {
    {
        const std::list<int> l1, l2;
        TC_ASSERT_EXPR(testComparisons(l1, l2, true, false));
    }
    {
        const std::list<int> l1(1, 1), l2(1, 1);
        TC_ASSERT_EXPR(testComparisons(l1, l2, true, false));
    }
    {
        int items[3] = {1, 2, 3};
        const std::list<int> l1(items, items + 3);
        const std::list<int> l2(items, items + 3);
        TC_ASSERT_EXPR(testComparisons(l1, l2, true, false));
    }
    {
        const std::list<int> l1(1, 1), l2;
        TC_ASSERT_EXPR(testComparisons(l1, l2, false, false));
    }
    {
        const std::list<int> l1(1, 1), l2(1, 2);
        TC_ASSERT_EXPR(testComparisons(l1, l2, false, true));
    }
    {
        int items1[2] = {1, 2};
        int items2[2] = {1, 3};
        const std::list<int> l1(items1, items1 + 2);
        const std::list<int> l2(items2, items2 + 2);
        TC_ASSERT_EXPR(testComparisons(l1, l2, false, true));
    }
    {
        int items1[2] = {2, 2};
        int items2[2] = {1, 3};
        const std::list<int> l1(items1, items1 + 2);
        const std::list<int> l2(items2, items2 + 2);
        TC_ASSERT_EXPR(testComparisons(l1, l2, false, false));
    }
    {
        const std::list<LessAndEqComp> l1, l2;
        TC_ASSERT_EXPR(testComparisons(l1, l2, true, false));
    }
    {
        const std::list<LessAndEqComp> l1(1, LessAndEqComp(1));
        const std::list<LessAndEqComp> l2(1, LessAndEqComp(1));
        TC_ASSERT_EXPR(testComparisons(l1, l2, true, false));
    }
    {
        LessAndEqComp items[3] = {LessAndEqComp(1), LessAndEqComp(2), LessAndEqComp(3)};
        const std::list<LessAndEqComp> l1(items, items + 3);
        const std::list<LessAndEqComp> l2(items, items + 3);
        TC_ASSERT_EXPR(testComparisons(l1, l2, true, false));
    }
    {
        const std::list<LessAndEqComp> l1(1, LessAndEqComp(1));
        const std::list<LessAndEqComp> l2;
        TC_ASSERT_EXPR(testComparisons(l1, l2, false, false));
    }
    {
        const std::list<LessAndEqComp> l1(1, LessAndEqComp(1));
        const std::list<LessAndEqComp> l2(1, LessAndEqComp(2));
        TC_ASSERT_EXPR(testComparisons(l1, l2, false, true));
    }
    {
        LessAndEqComp items1[2] = {LessAndEqComp(1), LessAndEqComp(2)};
        LessAndEqComp items2[2] = {LessAndEqComp(1), LessAndEqComp(3)};
        const std::list<LessAndEqComp> l1(items1, items1 + 2);
        const std::list<LessAndEqComp> l2(items2, items2 + 2);
        TC_ASSERT_EXPR(testComparisons(l1, l2, false, true));
    }
    {
        LessAndEqComp items1[2] = {LessAndEqComp(2), LessAndEqComp(2)};
        LessAndEqComp items2[2] = {LessAndEqComp(1), LessAndEqComp(3)};
        const std::list<LessAndEqComp> l1(items1, items1 + 2);
        const std::list<LessAndEqComp> l2(items2, items2 + 2);
        TC_ASSERT_EXPR(testComparisons(l1, l2, false, false));
    }
    return 0;
}
