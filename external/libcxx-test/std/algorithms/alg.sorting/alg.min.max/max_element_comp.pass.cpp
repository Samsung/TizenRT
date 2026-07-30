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
// template<ForwardIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires CopyConstructible<Compare>
//   Iter
//   max_element(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <functional>
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
    Iter i = std::max_element(first, last, std::greater<int>());
    if (first != last)
    {
        for (Iter j = first; j != last; ++j)
            TC_ASSERT_EXPR(!std::greater<int>()(*i, *j));
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
void test_max_element_comp_size_0() {
    test_for_size<Iter>(0);
}

template <class Iter>
void test_max_element_comp_size_1() {
    test_for_size<Iter>(1);
}

template <class Iter>
void test_max_element_comp_size_2() {
    test_for_size<Iter>(2);
}

template <class Iter>
void test_max_element_comp_size_3() {
    test_for_size<Iter>(3);
}

template <class Iter>
void test_max_element_comp_size_10() {
    test_for_size<Iter>(10);
}

template <class Iter>
void test_max_element_comp_size_1000() {
    test_for_size<Iter>(1000);
}

template <class Iter>
void test_all_max_element_comp_sizes() {
    test_max_element_comp_size_0<Iter>();
    test_max_element_comp_size_1<Iter>();
    test_max_element_comp_size_2<Iter>();
    test_max_element_comp_size_3<Iter>();
    test_max_element_comp_size_10<Iter>();
    test_max_element_comp_size_1000<Iter>();
}

template <class Iter, class Pred>
void test_eq0(Iter first, Iter last, Pred p)
{
    TC_ASSERT_EXPR(first == std::max_element(first, last, p));
}

void test_eq()
{
    const int N = 10;
    int* a = new int[N];
    for (int i = 0; i < N; ++i)
        a[i] = 10;
    test_eq0(a, a+N, std::less<int>());
    test_eq0(a, a+N, std::greater<int>());
    delete [] a;
}

#if TEST_STD_VER >= 14
constexpr int il[] = { 2, 4, 6, 8, 7, 5, 3, 1 };
struct less { constexpr bool operator ()( const int &x, const int &y) const { return x < y; }};
#endif

void constexpr_test()
{
#if TEST_STD_VER >= 14
    constexpr auto p = std::max_element(il, il+8, less());
    static_assert ( *p == 8, "" );
#endif
}

void run_all_max_element_comp_tests() {
    test_all_max_element_comp_sizes<forward_iterator<const int*> >();
    test_all_max_element_comp_sizes<bidirectional_iterator<const int*> >();
    test_all_max_element_comp_sizes<random_access_iterator<const int*> >();
    test_all_max_element_comp_sizes<const int*>();

    test_eq();
    constexpr_test();
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_min_max_max_element_comp(void) {
    run_all_max_element_comp_tests();

    TC_SUCCESS_RESULT();

    return 0;
}
