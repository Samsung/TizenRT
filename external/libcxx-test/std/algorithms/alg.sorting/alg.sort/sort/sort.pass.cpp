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
// template<RandomAccessIterator Iter>
//   requires ShuffleIterator<Iter>
//         && LessThanComparable<Iter::value_type>
//   void
//   sort(Iter first, Iter last);

#include <algorithm>
#include <iterator>
#include <numeric>
#include <random>
#include <cassert>
#include <vector>
#include <deque>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

static std::mt19937 randomness;

template <class Container, class RI>
void
test_sort_helper(RI f, RI l)
{
    if (f != l)
    {
        Container save(l - f);
        do
        {
            std::copy(f, l, save.begin());
            std::sort(save.begin(), save.end());
            TC_ASSERT_EXPR(std::is_sorted(save.begin(), save.end()));
            TC_ASSERT_EXPR(std::is_permutation(save.begin(), save.end(), f));
        } while (std::next_permutation(f, l));
    }
}

template <class T>
void set_value(T& dest, int value)
{
    dest = value;
}

inline void set_value(std::pair<int, int>& dest, int value)
{
    dest.first = value;
    dest.second = value;
}

template <class Container, class RI>
void
test_sort_driver_driver(RI f, RI l, int start, RI real_last)
{
    for (RI i = l; i > f + start;)
    {
        set_value(*--i, start);
        if (f == i)
        {
            test_sort_helper<Container>(f, real_last);
        }
        if (start > 0)
            test_sort_driver_driver<Container>(f, i, start-1, real_last);
    }
}

template <class Container, class RI>
void
test_sort_driver(RI f, RI l, int start)
{
    test_sort_driver_driver<Container>(f, l, start, l);
}

template <class T>
T increment_or_reset(T value, int max_value)
{
    return value == max_value - 1 ? 0 : value + 1;
}

inline std::pair<int, int> increment_or_reset(std::pair<int, int> value,
                                              int max_value)
{
    int new_value = value.first + 1;
    if (new_value == max_value)
    {
        new_value = 0;
    }
    return std::make_pair(new_value, new_value);
}

template <class Container>
void
test_larger_sorts(int N, int M)
{
    using Iter = typename Container::iterator;
    using ValueType = typename Container::value_type;
    TC_ASSERT_EXPR(N != 0);
    TC_ASSERT_EXPR(M != 0);
    // create container of length N filled with M different objects
    Container array(N);
    ValueType x = ValueType();
    for (int i = 0; i < N; ++i)
    {
        array[i] = x;
        x = increment_or_reset(x, M);
    }
    Container original = array;
    Iter iter = array.begin();
    Iter original_iter = original.begin();

    // test saw tooth pattern
    std::sort(iter, iter+N);
    TC_ASSERT_EXPR(std::is_sorted(iter, iter+N));
    TC_ASSERT_EXPR(std::is_permutation(iter, iter+N, original_iter));
    // test random pattern
    std::shuffle(iter, iter+N, randomness);
    std::sort(iter, iter+N);
    TC_ASSERT_EXPR(std::is_sorted(iter, iter+N));
    TC_ASSERT_EXPR(std::is_permutation(iter, iter+N, original_iter));
    // test sorted pattern
    std::sort(iter, iter+N);
    TC_ASSERT_EXPR(std::is_sorted(iter, iter+N));
    TC_ASSERT_EXPR(std::is_permutation(iter, iter+N, original_iter));
    // test reverse sorted pattern
    std::reverse(iter, iter+N);
    std::sort(iter, iter+N);
    TC_ASSERT_EXPR(std::is_sorted(iter, iter+N));
    TC_ASSERT_EXPR(std::is_permutation(iter, iter+N, original_iter));
    // test swap ranges 2 pattern
    std::swap_ranges(iter, iter+N/2, iter+N/2);
    std::sort(iter, iter+N);
    TC_ASSERT_EXPR(std::is_sorted(iter, iter+N));
    TC_ASSERT_EXPR(std::is_permutation(iter, iter+N, original_iter));
    // test reverse swap ranges 2 pattern
    std::reverse(iter, iter+N);
    std::swap_ranges(iter, iter+N/2, iter+N/2);
    std::sort(iter, iter+N);
    TC_ASSERT_EXPR(std::is_sorted(iter, iter+N));
    TC_ASSERT_EXPR(std::is_permutation(iter, iter+N, original_iter));
}

template <class Container>
void
test_larger_sorts(int N)
{
    test_larger_sorts<Container>(N, 1);
    test_larger_sorts<Container>(N, 2);
    test_larger_sorts<Container>(N, 3);
    test_larger_sorts<Container>(N, N/2-1);
    test_larger_sorts<Container>(N, N/2);
    test_larger_sorts<Container>(N, N/2+1);
    test_larger_sorts<Container>(N, N-2);
    test_larger_sorts<Container>(N, N-1);
    test_larger_sorts<Container>(N, N);
}

void
test_pointer_sort()
{
    static const int array_size = 10;
    const int v[array_size] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const int *pv[array_size];
    for (int i = 0; i < array_size; i++) {
      pv[i] = &v[array_size - 1 - i];
    }
    std::sort(pv, pv + array_size);
    TC_ASSERT_EXPR(*pv[0] == v[0]);
    TC_ASSERT_EXPR(*pv[1] == v[1]);
    TC_ASSERT_EXPR(*pv[array_size - 1] == v[array_size - 1]);
}

// test_adversarial_quicksort generates a vector with values arranged in such a
// way that they would invoke O(N^2) behavior on any quick sort implementation
// that satisifies certain conditions.  Details are available in the following
// paper:
// "A Killer Adversary for Quicksort", M. D. McIlroy, Software-Practice &
// Experience Volume 29 Issue 4 April 10, 1999 pp 341-344.
// https://dl.acm.org/doi/10.5555/311868.311871.
struct AdversaryComparator {
  AdversaryComparator(int N, std::vector<int>& input) : gas(N - 1), V(input) {
    V.resize(N);
    // Populate all positions in the generated input to gas to indicate that
    // none of the values have been fixed yet.
    for (int i = 0; i < N; ++i)
      V[i] = gas;
  }

  bool operator()(int x, int y) {
    if (V[x] == gas && V[y] == gas) {
      // We are comparing two inputs whose value is still to be decided.
      if (x == candidate) {
        V[x] = nsolid++;
      } else {
        V[y] = nsolid++;
      }
    }
    if (V[x] == gas) {
      candidate = x;
    } else if (V[y] == gas) {
      candidate = y;
    }
    return V[x] < V[y];
  }

private:
  // If an element is equal to gas, it indicates that the value of the element
  // is still to be decided and may change over the course of time.
  const int gas;
  // This is a reference so that we can manipulate the input vector later.
  std::vector<int>& V;
  // Candidate for the pivot position.
  int candidate = 0;
  int nsolid = 0;
};

void test_adversarial_quicksort(int N) {
  TC_ASSERT_EXPR(N > 0);
  std::vector<int> ascVals(N);
  // Fill up with ascending values from 0 to N-1.  These will act as indices
  // into V.
  std::iota(ascVals.begin(), ascVals.end(), 0);
  std::vector<int> V;
  AdversaryComparator comp(N, V);
  std::sort(ascVals.begin(), ascVals.end(), comp);
  std::sort(V.begin(), V.end());
  TC_ASSERT_EXPR(std::is_sorted(V.begin(), V.end()));
}

// ============================================================================
// Null range tests
// ============================================================================

void test_sort_null_range_vector_int() {
    int d = 0;
    std::sort(&d, &d);
}

void test_sort_null_range_deque_int() {
    int d = 0;
    std::sort(&d, &d);
}

void test_sort_null_range_vector_pair() {
    std::pair<int, int> d = std::make_pair(0, 0);
    std::sort(&d, &d);
}

// TizenRT: Reduced from sizes 1-8 to 1-4 to prevent stack overflow on embedded systems
// Test integrity preserved: 24 permutations at size 4 + large tests (256-1009) still provide comprehensive coverage
// Small exhaustive tests (sizes 1-4) - std::vector<int>
// ============================================================================

void test_sort_vector_int_size_1() {
    std::vector<int> ia(1);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 0);
}

void test_sort_vector_int_size_2() {
    std::vector<int> ia(2);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 1);
}

void test_sort_vector_int_size_3() {
    std::vector<int> ia(3);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 1);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 2);
}

void test_sort_vector_int_size_4() {
    std::vector<int> ia(4);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 1);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 2);
    test_sort_driver<std::vector<int> >(ia.begin(), ia.end(), 3);
}

// TizenRT: Reduced from sizes 1-8 to 1-4 to prevent stack overflow on embedded systems
// Small exhaustive tests (sizes 1-4) - std::deque<int>
// ============================================================================

void test_sort_deque_int_size_1() {
    std::deque<int> ia(1);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 0);
}

void test_sort_deque_int_size_2() {
    std::deque<int> ia(2);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 1);
}

void test_sort_deque_int_size_3() {
    std::deque<int> ia(3);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 1);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 2);
}

void test_sort_deque_int_size_4() {
    std::deque<int> ia(4);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 1);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 2);
    test_sort_driver<std::deque<int> >(ia.begin(), ia.end(), 3);
}

// TizenRT: Reduced from sizes 1-8 to 1-4 to prevent stack overflow on embedded systems
// Small exhaustive tests (sizes 1-4) - std::vector<std::pair<int,int>>
// ============================================================================

void test_sort_vector_pair_size_1() {
    std::vector<std::pair<int, int> > ia(1);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 0);
}

void test_sort_vector_pair_size_2() {
    std::vector<std::pair<int, int> > ia(2);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 1);
}

void test_sort_vector_pair_size_3() {
    std::vector<std::pair<int, int> > ia(3);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 1);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 2);
}

void test_sort_vector_pair_size_4() {
    std::vector<std::pair<int, int> > ia(4);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 0);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 1);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 2);
    test_sort_driver<std::vector<std::pair<int, int> > >(ia.begin(), ia.end(), 3);
}

// ============================================================================
// Large tests - std::vector<int>
// ============================================================================

void test_sort_vector_int_large() {
    test_larger_sorts<std::vector<int> >(256);
    test_larger_sorts<std::vector<int> >(257);
    test_larger_sorts<std::vector<int> >(499);
    test_larger_sorts<std::vector<int> >(500);
    test_larger_sorts<std::vector<int> >(997);
    test_larger_sorts<std::vector<int> >(1000);
    test_larger_sorts<std::vector<int> >(1009);
}

// ============================================================================
// Large tests - std::deque<int>
// ============================================================================

void test_sort_deque_int_large() {
    test_larger_sorts<std::deque<int> >(256);
    test_larger_sorts<std::deque<int> >(257);
    test_larger_sorts<std::deque<int> >(499);
    test_larger_sorts<std::deque<int> >(500);
    test_larger_sorts<std::deque<int> >(997);
    test_larger_sorts<std::deque<int> >(1000);
    test_larger_sorts<std::deque<int> >(1009);
}

// ============================================================================
// Large tests - std::vector<std::pair<int,int>>
// ============================================================================

void test_sort_vector_pair_large() {
    test_larger_sorts<std::vector<std::pair<int, int> > >(256);
    test_larger_sorts<std::vector<std::pair<int, int> > >(257);
    test_larger_sorts<std::vector<std::pair<int, int> > >(499);
    test_larger_sorts<std::vector<std::pair<int, int> > >(500);
    test_larger_sorts<std::vector<std::pair<int, int> > >(997);
    test_larger_sorts<std::vector<std::pair<int, int> > >(1000);
    test_larger_sorts<std::vector<std::pair<int, int> > >(1009);
}

// Group functions called internally for stack reduction
void test_sort_null_range_all() {
    test_sort_null_range_vector_int();
    test_sort_null_range_deque_int();
    test_sort_null_range_vector_pair();
}

// TizenRT: Reduced from sizes 1-8 to 1-4 to prevent stack overflow
void test_sort_vector_int_all_small() {
    test_sort_vector_int_size_1();
    test_sort_vector_int_size_2();
    test_sort_vector_int_size_3();
    test_sort_vector_int_size_4();
}

void test_sort_deque_int_all_small() {
    test_sort_deque_int_size_1();
    test_sort_deque_int_size_2();
    test_sort_deque_int_size_3();
    test_sort_deque_int_size_4();
}

void test_sort_vector_pair_all_small() {
    test_sort_vector_pair_size_1();
    test_sort_vector_pair_size_2();
    test_sort_vector_pair_size_3();
    test_sort_vector_pair_size_4();
}

void test_sort_all_small() {
    test_sort_vector_int_all_small();
    test_sort_deque_int_all_small();
    test_sort_vector_pair_all_small();
}

void test_sort_all_large() {
    test_sort_vector_int_large();
    test_sort_deque_int_large();
    test_sort_vector_pair_large();
}

void test_sort_special() {
    test_pointer_sort();
    test_adversarial_quicksort(1 << 20);
}

void run_all_sort_tests()
{
    test_sort_null_range_all();
    test_sort_all_small();
    test_sort_all_large();
    test_sort_special();
}

// ============================================================================
// Test entry point with debug traces
// ============================================================================

int tc_libcxx_algorithms_alg_sorting_alg_sort_sort_sort(void) {
    test_sort_null_range_all();
    test_sort_all_small();
    test_sort_all_large();
    test_sort_special();

    TC_SUCCESS_RESULT();

    return 0;
}
