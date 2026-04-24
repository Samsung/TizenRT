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

// template<BidirectionalIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires ShuffleIterator<Iter>
//         && CopyConstructible<Compare>
//   void
//   inplace_merge(Iter first, Iter middle, Iter last, Compare comp);

#include <algorithm>
#include <functional>
#include <random>
#include <cassert>

#include "test_macros.h"

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
#include "libcxx_tc_common.h"

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
test(unsigned N)
{
    test_one<Iter>(N, 0);
    test_one<Iter>(N, N/4);
    test_one<Iter>(N, N/2);
    test_one<Iter>(N, 3*N/4);
    test_one<Iter>(N, N);
}

template <class Iter>
void
test()
{
    test_one<Iter>(0, 0);
    test_one<Iter>(1, 0);
    test_one<Iter>(1, 1);
    test_one<Iter>(2, 0);
    test_one<Iter>(2, 1);
    test_one<Iter>(2, 2);
    test_one<Iter>(3, 0);
    test_one<Iter>(3, 1);
    test_one<Iter>(3, 2);
    test_one<Iter>(3, 3);
    test<Iter>(4);
    test<Iter>(20);
    test<Iter>(100);
    test<Iter>(1000);
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

int tc_algorithms_alg_sorting_alg_merge_inplace_merge_comp(void) {
    test<bidirectional_iterator<int*> >();
    test<random_access_iterator<int*> >();
    test<int*>();

#if TEST_STD_VER >= 11
    test<bidirectional_iterator<S*> >();
    test<random_access_iterator<S*> >();
    test<S*>();

    {
    int N = 100;
    unsigned M = 50;
    std::unique_ptr<int>* ia = new std::unique_ptr<int>[N];
    for (int i = 0; i < N; ++i)
        ia[i].reset(new int(i));
    std::shuffle(ia, ia+N, randomness);
    std::sort(ia, ia+M, indirect_less());
    std::sort(ia+M, ia+N, indirect_less());
    std::inplace_merge(ia, ia+M, ia+N, indirect_less());
    if(N > 0)
    {
        TC_ASSERT_EXPR(*ia[0] == 0);
        TC_ASSERT_EXPR(*ia[N-1] == N-1);
        TC_ASSERT_EXPR(std::is_sorted(ia, ia+N, indirect_less()));
    }
    delete [] ia;
    }
#endif // TEST_STD_VER >= 11

    test_PR31166();

  return 0;
}
