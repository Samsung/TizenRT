
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
// template<BidirectionalIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires ShuffleIterator<Iter>
//         && CopyConstructible<Compare>
//   void
//   inplace_merge(Iter first, Iter middle, Iter last, Compare comp);

// TizenRT: Disable count_new.h memory tracking to prevent libsupc++ conflicts
#define DISABLE_NEW_COUNT

#include <algorithm>
#include <functional>
#include <random>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER >= 11
#include <memory>

struct indirect_less
{
    template <class P>
    bool operator()(const P& x, const P& y)
        {return *x < *y;}
};

struct S {
    S() : i_(0) {}
    S(int i) : i_(i) {}

    S(const S&  rhs) : i_(rhs.i_) {}
    S(      S&& rhs) : i_(rhs.i_) { rhs.i_ = -1; }

    S& operator =(const S&  rhs) { i_ = rhs.i_;              return *this; }
    S& operator =(      S&& rhs) { i_ = rhs.i_; rhs.i_ = -2; TC_ASSERT_EXPR(this != &rhs); return *this; }
    S& operator =(int i)         { i_ = i;                   return *this; }

    bool operator  <(const S&  rhs) const { return i_ < rhs.i_; }
    bool operator  >(const S&  rhs) const { return i_ > rhs.i_; }
    bool operator ==(const S&  rhs) const { return i_ == rhs.i_; }
    bool operator ==(int i)         const { return i_ == i; }

    void set(int i) { i_ = i; }

    int i_;
    };


#endif // TEST_STD_VER >= 11

#include "test_iterators.h"
#include "counting_predicates.h"

namespace {
std::mt19937 randomness;

template <class Iter>
void
test_one(unsigned N, unsigned M)
{
    TC_ASSERT_EXPR(M <= N);
    typedef typename std::iterator_traits<Iter>::value_type value_type;
    value_type* ia = new value_type[N];
    for (unsigned i = 0; i < N; ++i)
        ia[i] = i;
    std::shuffle(ia, ia+N, randomness);
    std::sort(ia, ia+M, std::greater<value_type>());
    std::sort(ia+M, ia+N, std::greater<value_type>());
    binary_counting_predicate<std::greater<value_type>, value_type, value_type> pred((std::greater<value_type>()));
    std::inplace_merge(Iter(ia), Iter(ia+M), Iter(ia+N), std::ref(pred));
    if(N > 0)
    {
        TC_ASSERT_EXPR(ia[0] == static_cast<int>(N)-1);
        TC_ASSERT_EXPR(ia[N-1] == 0);
        TC_ASSERT_EXPR(std::is_sorted(ia, ia+N, std::greater<value_type>()));
#if !_LIBCPP_ENABLE_DEBUG_MODE
        TC_ASSERT_EXPR(pred.count() <= (N-1));
#endif
    }
    delete [] ia;
}

template <class Iter>
void
test_for_size(unsigned N)
{
    test_one<Iter>(N, 0);
    test_one<Iter>(N, N/4);
    test_one<Iter>(N, N/2);
    test_one<Iter>(N, 3*N/4);
    test_one<Iter>(N, N);
}

template <class Iter>
void test_size_0() {
    test_one<Iter>(0, 0);
}

template <class Iter>
void test_size_1() {
    test_one<Iter>(1, 0);
    test_one<Iter>(1, 1);
}

template <class Iter>
void test_size_2() {
    test_one<Iter>(2, 0);
    test_one<Iter>(2, 1);
    test_one<Iter>(2, 2);
}

template <class Iter>
void test_size_3() {
    test_one<Iter>(3, 0);
    test_one<Iter>(3, 1);
    test_one<Iter>(3, 2);
    test_one<Iter>(3, 3);
}

template <class Iter>
void test_all_sizes() {
    test_size_0<Iter>();
    test_size_1<Iter>();
    test_size_2<Iter>();
    test_size_3<Iter>();
    test_for_size<Iter>(4);
    test_for_size<Iter>(20);
    test_for_size<Iter>(100);
    test_for_size<Iter>(1000);
}

struct less_by_first {
  template <typename Pair>
  bool operator()(const Pair& lhs, const Pair& rhs) {
    return std::less<typename Pair::first_type>()(lhs.first, rhs.first);
  }
};

void test_PR31166 ()
{
    typedef std::pair<int, int> P;
    typedef std::vector<P> V;
    P vec[5] = {P(1, 0), P(2, 0), P(2, 1), P(2, 2), P(2, 3)};
    for ( int i = 0; i < 5; ++i ) {
        V res(vec, vec + 5);
        std::inplace_merge(res.begin(), res.begin() + i, res.end(), less_by_first());
        TC_ASSERT_EXPR(res.size() == 5);
        TC_ASSERT_EXPR(std::equal(res.begin(), res.end(), vec));
    }
}

#if TEST_STD_VER >= 11
void test_S_struct() {
    test_all_sizes<bidirectional_iterator<S*> >();
    test_all_sizes<random_access_iterator<S*> >();
    test_all_sizes<S*>();
}

void test_unique_ptr() {
    unsigned N = 100;
    unsigned M = 50;
    std::unique_ptr<int>* ia = new std::unique_ptr<int>[N];
    for (unsigned i = 0; i < N; ++i)
        ia[i].reset(new int(i));
    std::shuffle(ia, ia+N, randomness);
    std::sort(ia, ia+M, indirect_less());
    std::sort(ia+M, ia+N, indirect_less());
    std::inplace_merge(ia, ia+M, ia+N, indirect_less());
    if(N > 0)
    {
        TC_ASSERT_EXPR(*ia[0] == 0);
        TC_ASSERT_EXPR(*ia[N-1] == static_cast<int>(N)-1);
        TC_ASSERT_EXPR(std::is_sorted(ia, ia+N, indirect_less()));
    }
    delete [] ia;
}
#endif

void run_all_inplace_merge_comp_tests() {
    test_all_sizes<bidirectional_iterator<int*> >();
    test_all_sizes<random_access_iterator<int*> >();
    test_all_sizes<int*>();

#if TEST_STD_VER >= 11
    test_S_struct();
    test_unique_ptr();
#endif

    test_PR31166();
}

} // namespace

int tc_libcxx_algorithms_alg_sorting_alg_merge_inplace_merge_comp(void) {
    run_all_inplace_merge_comp_tests();

    TC_SUCCESS_RESULT();

    return 0;
}
