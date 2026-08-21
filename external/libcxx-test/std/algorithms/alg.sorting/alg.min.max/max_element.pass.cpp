//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>
//
// template<ForwardIterator Iter>
//   requires LessThanComparable<Iter::value_type>
//   Iter
//   max_element(Iter first, Iter last);

#include <algorithm>
#include <random>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

namespace {
std::mt19937 randomness;

template <class Iter>
void
test(Iter first, Iter last)
{
    Iter i = std::max_element(first, last);
    if (first != last)
    {
        for (Iter j = first; j != last; ++j)
            TC_ASSERT_EXPR(!(*i < *j));
    }
    else
        TC_ASSERT_EXPR(i == last);
}

template <class Iter>
void
test_for_size(int N)
{
    int* a = new int[N];
    for (int i = 0; i < N; ++i)
        a[i] = i;
    std::shuffle(a, a+N, randomness);
    test(Iter(a), Iter(a+N));
    delete [] a;
}

template <class Iter>
void test_max_element_size_0() {
    test_for_size<Iter>(0);
}

template <class Iter>
void test_max_element_size_1() {
    test_for_size<Iter>(1);
}

template <class Iter>
void test_max_element_size_2() {
    test_for_size<Iter>(2);
}

template <class Iter>
void test_max_element_size_3() {
    test_for_size<Iter>(3);
}

template <class Iter>
void test_max_element_size_10() {
    test_for_size<Iter>(10);
}

template <class Iter>
void test_max_element_size_1000() {
    test_for_size<Iter>(1000);
}

template <class Iter>
void test_all_max_element_sizes() {
    test_max_element_size_0<Iter>();
    test_max_element_size_1<Iter>();
    test_max_element_size_2<Iter>();
    test_max_element_size_3<Iter>();
    test_max_element_size_10<Iter>();
    test_max_element_size_1000<Iter>();
}

#if TEST_STD_VER >= 14
constexpr int il[] = { 2, 4, 6, 8, 7, 5, 3, 1 };
#endif

void constexpr_test()
{
#if TEST_STD_VER >= 14
    constexpr auto p = std::max_element(il,il+8);
    static_assert ( *p == 8, "" );
#endif
}

void run_all_max_element_tests() {
    test_all_max_element_sizes<forward_iterator<const int*> >();
    test_all_max_element_sizes<bidirectional_iterator<const int*> >();
    test_all_max_element_sizes<random_access_iterator<const int*> >();
    test_all_max_element_sizes<const int*>();

    constexpr_test();
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_min_max_max_element(void) {
    run_all_max_element_tests();

    TC_SUCCESS_RESULT();

    return 0;
}
