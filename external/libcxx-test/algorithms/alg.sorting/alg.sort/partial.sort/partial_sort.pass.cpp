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
// <algorithm>

// template<RandomAccessIterator Iter>
//   requires ShuffleIterator<Iter> && LessThanComparable<Iter::value_type>
//   constexpr void  // constexpr in C++20
//   partial_sort(Iter first, Iter middle, Iter last);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

template<class T, class Iter>
TEST_CONSTEXPR_CXX20 bool test()
{
    int orig[15] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};
    T work[15] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};
    for (int n = 0; n < 15; ++n) {
        for (int m = 0; m <= n; ++m) {
            std::partial_sort(Iter(work), Iter(work+m), Iter(work+n));
            TC_ASSERT_EXPR(std::is_sorted(work, work+m));
            TC_ASSERT_EXPR(std::is_permutation(work, work+n, orig));
            // No element in the unsorted portion is less than any element in the sorted portion.
            for (int i = m; i < n; ++i) {
                TC_ASSERT_EXPR(m == 0 || !(work[i] < work[m-1]));
            }
            std::copy(orig, orig+15, work);
        }
    }

    {
        T input[] = {3, 4, 2, 5, 1};
        std::partial_sort(Iter(input), Iter(input + 3), Iter(input + 5));
        TC_ASSERT_EXPR(input[0] == 1);
        TC_ASSERT_EXPR(input[1] == 2);
        TC_ASSERT_EXPR(input[2] == 3);
        TC_ASSERT_EXPR(input[3] + input[4] == 4 + 5);
    }
    return true;
}

int tc_algorithms_alg_sorting_alg_sort_partial_sort_partial_sort(void) {
    int i = 42;
    std::partial_sort(&i, &i, &i);  // no-op
    TC_ASSERT_EXPR(i == 42);

    test<int, random_access_iterator<int*> >();
    test<int, int*>();

#if TEST_STD_VER >= 11
    test<MoveOnly, random_access_iterator<MoveOnly*>>();
    test<MoveOnly, MoveOnly*>();
#endif

#if TEST_STD_VER >= 20
    static_assert(test<int, random_access_iterator<int*>>());
    static_assert(test<int, int*>());
    static_assert(test<MoveOnly, random_access_iterator<MoveOnly*>>());
    static_assert(test<MoveOnly, MoveOnly*>());
#endif

    return 0;
}
