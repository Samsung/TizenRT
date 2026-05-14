//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<RandomAccessIterator Iter>
//   requires ShuffleIterator<Iter> && LessThanComparable<Iter::value_type>
//   constexpr void  // constexpr in C++20
//   pop_heap(Iter first, Iter last);

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
    T orig[15] = {9,6,9,5,5, 8,9,1,1,3, 5,3,4,7,2};
    T work[15] = {9,6,9,5,5, 8,9,1,1,3, 5,3,4,7,2};
    TC_ASSERT_EXPR(std::is_heap(orig, orig+15));
    for (int i = 15; i >= 1; --i) {
        std::pop_heap(Iter(work), Iter(work+i));
        TC_ASSERT_EXPR(std::is_heap(work, work+i-1));
        TC_ASSERT_EXPR(std::max_element(work, work+i-1) == work);
        TC_ASSERT_EXPR(std::is_permutation(work, work+15, orig));
    }
    TC_ASSERT_EXPR(std::is_sorted(work, work+15));

    {
        T input[] = {5, 4, 1, 2, 3};
        TC_ASSERT_EXPR(std::is_heap(input, input + 5));
        std::pop_heap(Iter(input), Iter(input + 5)); TC_ASSERT_EXPR(input[4] == 5);
        std::pop_heap(Iter(input), Iter(input + 4)); TC_ASSERT_EXPR(input[3] == 4);
        std::pop_heap(Iter(input), Iter(input + 3)); TC_ASSERT_EXPR(input[2] == 3);
        std::pop_heap(Iter(input), Iter(input + 2)); TC_ASSERT_EXPR(input[1] == 2);
        std::pop_heap(Iter(input), Iter(input + 1)); TC_ASSERT_EXPR(input[0] == 1);
    }
    return true;
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_heap_operations_pop_heap_pop_heap(void) {
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
