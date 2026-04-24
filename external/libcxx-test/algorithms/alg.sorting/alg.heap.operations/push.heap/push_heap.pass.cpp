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
//   push_heap(Iter first, Iter last);

#include <algorithm>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

template<class T, class Iter>
TEST_CONSTEXPR_CXX20 bool test()
{
    T orig[15] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};
    T work[15] = {3,1,4,1,5, 9,2,6,5,3, 5,8,9,7,9};
    for (int i = 1; i < 15; ++i) {
        std::push_heap(Iter(work), Iter(work+i));
        TC_ASSERT_EXPR(std::is_permutation(work, work+i, orig));
        TC_ASSERT_EXPR(std::is_heap(work, work+i));
    }

    {
        T input[] = {1, 3, 2, 5, 4};
        std::push_heap(Iter(input), Iter(input + 1)); TC_ASSERT_EXPR(input[0] == 1);
        std::push_heap(Iter(input), Iter(input + 2)); TC_ASSERT_EXPR(input[0] == 3);
        std::push_heap(Iter(input), Iter(input + 3)); TC_ASSERT_EXPR(input[0] == 3);
        std::push_heap(Iter(input), Iter(input + 4)); TC_ASSERT_EXPR(input[0] == 5);
        std::push_heap(Iter(input), Iter(input + 5)); TC_ASSERT_EXPR(input[0] == 5);
        TC_ASSERT_EXPR(std::is_heap(input, input + 5));
    }
    return true;
}

int tc_algorithms_alg_sorting_alg_heap_operations_push_heap_push_heap(void) {
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
