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
//   requires ShuffleIterator<Iter>
//         && LessThanComparable<Iter::value_type>
//   void
//   stable_sort(Iter first, Iter last);

#include <algorithm>
#include <cassert>
#include <iterator>
#include <random>

#include "count_new.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

std::mt19937 randomness;

template <class RI>
void
test_sort_helper(RI f, RI l)
{
    typedef typename std::iterator_traits<RI>::value_type value_type;
    typedef typename std::iterator_traits<RI>::difference_type difference_type;

    if (f != l)
    {
        difference_type len = l - f;
        value_type* save(new value_type[len]);
        do
        {
            std::copy(f, l, save);
            std::stable_sort(save, save+len);
            TC_ASSERT_EXPR(std::is_sorted(save, save+len));
        } while (std::next_permutation(f, l));
        delete [] save;
    }
}

template <class RI>
void
test_sort_driver_driver(RI f, RI l, int start, RI real_last)
{
    for (RI i = l; i > f + start;)
    {
        *--i = start;
        if (f == i)
        {
            test_sort_helper(f, real_last);
        }
    if (start > 0)
        test_sort_driver_driver(f, i, start-1, real_last);
    }
}

template <class RI>
void
test_sort_driver(RI f, RI l, int start)
{
    test_sort_driver_driver(f, l, start, l);
}

template <int sa>
void
test_sort_()
{
    int ia[sa];
    for (int i = 0; i < sa; ++i)
    {
        test_sort_driver(ia, ia+sa, i);
    }
}

void
test_larger_sorts(int N, int M)
{
    TC_ASSERT_EXPR(N != 0);
    TC_ASSERT_EXPR(M != 0);
    // create array length N filled with M different numbers
    int* array = new int[N];
    int x = 0;
    for (int i = 0; i < N; ++i)
    {
        array[i] = x;
        if (++x == M)
            x = 0;
    }
    // test saw tooth pattern
    std::stable_sort(array, array+N);
    TC_ASSERT_EXPR(std::is_sorted(array, array+N));
    // test random pattern
    std::shuffle(array, array+N, randomness);
    std::stable_sort(array, array+N);
    TC_ASSERT_EXPR(std::is_sorted(array, array+N));
    // test sorted pattern
    std::stable_sort(array, array+N);
    TC_ASSERT_EXPR(std::is_sorted(array, array+N));
    // test reverse sorted pattern
    std::reverse(array, array+N);
    std::stable_sort(array, array+N);
    TC_ASSERT_EXPR(std::is_sorted(array, array+N));
    // test swap ranges 2 pattern
    std::swap_ranges(array, array+N/2, array+N/2);
    std::stable_sort(array, array+N);
    TC_ASSERT_EXPR(std::is_sorted(array, array+N));
    // test reverse swap ranges 2 pattern
    std::reverse(array, array+N);
    std::swap_ranges(array, array+N/2, array+N/2);
    std::stable_sort(array, array+N);
    TC_ASSERT_EXPR(std::is_sorted(array, array+N));
    delete [] array;
}

void
test_larger_sorts(int N)
{
    test_larger_sorts(N, 1);
    test_larger_sorts(N, 2);
    test_larger_sorts(N, 3);
    test_larger_sorts(N, N/2-1);
    test_larger_sorts(N, N/2);
    test_larger_sorts(N, N/2+1);
    test_larger_sorts(N, N-2);
    test_larger_sorts(N, N-1);
    test_larger_sorts(N, N);
}

int tc_algorithms_alg_sorting_alg_sort_stable_sort_stable_sort(void) {
    // test null range
    int d = 0;
    std::stable_sort(&d, &d);
    // exhaustively test all possibilities up to length 8
    test_sort_<1>();
    test_sort_<2>();
    test_sort_<3>();
    test_sort_<4>();
    test_sort_<5>();
    test_sort_<6>();
    test_sort_<7>();
    test_sort_<8>();

    test_larger_sorts(256);
    test_larger_sorts(257);
    test_larger_sorts(499);
    test_larger_sorts(500);
    test_larger_sorts(997);
    test_larger_sorts(1000);
    test_larger_sorts(1009);

#if !defined(TEST_HAS_NO_EXCEPTIONS)
    { // check that the algorithm works without memory
        std::vector<int> vec(150, 3);
        getGlobalMemCounter()->throw_after = 0;
        std::stable_sort(vec.begin(), vec.end());
    }
#endif // !defined(TEST_HAS_NO_EXCEPTIONS)

  return 0;
}
