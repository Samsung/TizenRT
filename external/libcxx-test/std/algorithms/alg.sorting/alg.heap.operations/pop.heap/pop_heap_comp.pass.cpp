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
 ****************************************************************************///
//
// <algorithm>

// template<RandomAccessIterator Iter>
//   requires ShuffleIterator<Iter> && LessThanComparable<Iter::value_type>
//   constexpr void  // constexpr in C++20
//   pop_heap(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <cassert>
#include <functional>

#include "test_macros.h"
#include "test_iterators.h"
#include "MoveOnly.h"
#include "libcxx_tc_common.h"

namespace {
template<class T, class Iter>
TEST_CONSTEXPR_CXX20 bool test()
{
    T orig[15] = {1,1,2,3,3, 8,4,6,5,5, 5,9,9,7,9};
    T work[15] = {1,1,2,3,3, 8,4,6,5,5, 5,9,9,7,9};
    TC_ASSERT_EXPR(std::is_heap(orig, orig+15, std::greater<T>()));
    for (int i = 15; i >= 1; --i) {
        std::pop_heap(Iter(work), Iter(work+i), std::greater<T>());
        TC_ASSERT_EXPR(std::is_heap(work, work+i-1, std::greater<T>()));
        TC_ASSERT_EXPR(std::min_element(work, work+i-1) == work);
        TC_ASSERT_EXPR(std::is_permutation(work, work+15, orig));
    }
    TC_ASSERT_EXPR(std::is_sorted(work, work+15, std::greater<T>()));

    {
        T input[] = {1, 2, 5, 4, 3};
        TC_ASSERT_EXPR(std::is_heap(input, input + 5, std::greater<T>()));
        std::pop_heap(Iter(input), Iter(input + 5), std::greater<T>()); TC_ASSERT_EXPR(input[4] == 1);
        std::pop_heap(Iter(input), Iter(input + 4), std::greater<T>()); TC_ASSERT_EXPR(input[3] == 2);
        std::pop_heap(Iter(input), Iter(input + 3), std::greater<T>()); TC_ASSERT_EXPR(input[2] == 3);
        std::pop_heap(Iter(input), Iter(input + 2), std::greater<T>()); TC_ASSERT_EXPR(input[1] == 4);
        std::pop_heap(Iter(input), Iter(input + 1), std::greater<T>()); TC_ASSERT_EXPR(input[0] == 5);
    }
    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_heap_operations_pop_heap_pop_heap_comp(void) {
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

    TC_SUCCESS_RESULT();


    return 0;
}
