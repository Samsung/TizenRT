//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<BidirectionalIterator Iter, Predicate<auto, Iter::value_type> Pred>
//   requires ShuffleIterator<Iter>
//         && CopyConstructible<Pred>
//   Iter
//   stable_partition(Iter first, Iter last, Pred pred);

#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

// Note: count_new.h removed - operator new/delete replacements conflict with libsupc++
#include "test_iterators.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace {
struct is_odd
{
    bool operator()(const int& i) const {return i & 1;}
};

struct odd_first
{
    bool operator()(const std::pair<int,int>& p) const
        {return p.first & 1;}
};

template <class Iter>
void
test()
{
  { // check mixed
    typedef std::pair<int,int> P;
    P array[] =
    {
        P(0, 1),
        P(0, 2),
        P(1, 1),
        P(1, 2),
        P(2, 1),
        P(2, 2),
        P(3, 1),
        P(3, 2),
        P(4, 1),
        P(4, 2)
    };
    const unsigned size = sizeof(array)/sizeof(array[0]);
    Iter r = std::stable_partition(Iter(array), Iter(array+size), odd_first());
    TC_ASSERT_EXPR(base(r) == array + 4);
    TC_ASSERT_EXPR(array[0] == P(1, 1));
    TC_ASSERT_EXPR(array[1] == P(1, 2));
    TC_ASSERT_EXPR(array[2] == P(3, 1));
    TC_ASSERT_EXPR(array[3] == P(3, 2));
    TC_ASSERT_EXPR(array[4] == P(0, 1));
    TC_ASSERT_EXPR(array[5] == P(0, 2));
    TC_ASSERT_EXPR(array[6] == P(2, 1));
    TC_ASSERT_EXPR(array[7] == P(2, 2));
    TC_ASSERT_EXPR(array[8] == P(4, 1));
    TC_ASSERT_EXPR(array[9] == P(4, 2));
  }
  {
    typedef std::pair<int,int> P;
    P array[] =
    {
        P(0, 1),
        P(0, 2),
        P(1, 1),
        P(1, 2),
        P(2, 1),
        P(2, 2),
        P(3, 1),
        P(3, 2),
        P(4, 1),
        P(4, 2)
    };
    const unsigned size = sizeof(array)/sizeof(array[0]);
    Iter r = std::stable_partition(Iter(array), Iter(array+size), odd_first());
    TC_ASSERT_EXPR(base(r) == array + 4);
    TC_ASSERT_EXPR(array[0] == P(1, 1));
    TC_ASSERT_EXPR(array[1] == P(1, 2));
    TC_ASSERT_EXPR(array[2] == P(3, 1));
    TC_ASSERT_EXPR(array[3] == P(3, 2));
    TC_ASSERT_EXPR(array[4] == P(0, 1));
    TC_ASSERT_EXPR(array[5] == P(0, 2));
    TC_ASSERT_EXPR(array[6] == P(2, 1));
    TC_ASSERT_EXPR(array[7] == P(2, 2));
    TC_ASSERT_EXPR(array[8] == P(4, 1));
    TC_ASSERT_EXPR(array[9] == P(4, 2));
    // check empty
    r = std::stable_partition(Iter(array), Iter(array), odd_first());
    TC_ASSERT_EXPR(base(r) == array);
    // check one true
    r = std::stable_partition(Iter(array), Iter(array+1), odd_first());
    TC_ASSERT_EXPR(base(r) == array+1);
    TC_ASSERT_EXPR(array[0] == P(1, 1));
    // check one false
    r = std::stable_partition(Iter(array+4), Iter(array+5), odd_first());
    TC_ASSERT_EXPR(base(r) == array+4);
    TC_ASSERT_EXPR(array[4] == P(0, 1));
  }
  { // check all false
    typedef std::pair<int,int> P;
    P array[] =
    {
        P(0, 1),
        P(0, 2),
        P(2, 1),
        P(2, 2),
        P(4, 1),
        P(4, 2),
        P(6, 1),
        P(6, 2),
        P(8, 1),
        P(8, 2)
    };
    const unsigned size = sizeof(array)/sizeof(array[0]);
    Iter r = std::stable_partition(Iter(array), Iter(array+size), odd_first());
    TC_ASSERT_EXPR(base(r) == array);
    TC_ASSERT_EXPR(array[0] == P(0, 1));
    TC_ASSERT_EXPR(array[1] == P(0, 2));
    TC_ASSERT_EXPR(array[2] == P(2, 1));
    TC_ASSERT_EXPR(array[3] == P(2, 2));
    TC_ASSERT_EXPR(array[4] == P(4, 1));
    TC_ASSERT_EXPR(array[5] == P(4, 2));
    TC_ASSERT_EXPR(array[6] == P(6, 1));
    TC_ASSERT_EXPR(array[7] == P(6, 2));
    TC_ASSERT_EXPR(array[8] == P(8, 1));
    TC_ASSERT_EXPR(array[9] == P(8, 2));
  }
  { // check all true
    typedef std::pair<int,int> P;
    P array[] =
    {
        P(1, 1),
        P(1, 2),
        P(3, 1),
        P(3, 2),
        P(5, 1),
        P(5, 2),
        P(7, 1),
        P(7, 2),
        P(9, 1),
        P(9, 2)
    };
    const unsigned size = sizeof(array)/sizeof(array[0]);
    Iter r = std::stable_partition(Iter(array), Iter(array+size), odd_first());
    TC_ASSERT_EXPR(base(r) == array + size);
    TC_ASSERT_EXPR(array[0] == P(1, 1));
    TC_ASSERT_EXPR(array[1] == P(1, 2));
    TC_ASSERT_EXPR(array[2] == P(3, 1));
    TC_ASSERT_EXPR(array[3] == P(3, 2));
    TC_ASSERT_EXPR(array[4] == P(5, 1));
    TC_ASSERT_EXPR(array[5] == P(5, 2));
    TC_ASSERT_EXPR(array[6] == P(7, 1));
    TC_ASSERT_EXPR(array[7] == P(7, 2));
    TC_ASSERT_EXPR(array[8] == P(9, 1));
    TC_ASSERT_EXPR(array[9] == P(9, 2));
  }
  { // check all false but first true
    typedef std::pair<int,int> P;
    P array[] =
    {
        P(1, 1),
        P(0, 2),
        P(2, 1),
        P(2, 2),
        P(4, 1),
        P(4, 2),
        P(6, 1),
        P(6, 2),
        P(8, 1),
        P(8, 2)
    };
    const unsigned size = sizeof(array)/sizeof(array[0]);
    Iter r = std::stable_partition(Iter(array), Iter(array+size), odd_first());
    TC_ASSERT_EXPR(base(r) == array + 1);
    TC_ASSERT_EXPR(array[0] == P(1, 1));
    TC_ASSERT_EXPR(array[1] == P(0, 2));
    TC_ASSERT_EXPR(array[2] == P(2, 1));
    TC_ASSERT_EXPR(array[3] == P(2, 2));
    TC_ASSERT_EXPR(array[4] == P(4, 1));
    TC_ASSERT_EXPR(array[5] == P(4, 2));
    TC_ASSERT_EXPR(array[6] == P(6, 1));
    TC_ASSERT_EXPR(array[7] == P(6, 2));
    TC_ASSERT_EXPR(array[8] == P(8, 1));
    TC_ASSERT_EXPR(array[9] == P(8, 2));
  }
  { // check all false but last true
    typedef std::pair<int,int> P;
    P array[] =
    {
        P(0, 1),
        P(0, 2),
        P(2, 1),
        P(2, 2),
        P(4, 1),
        P(4, 2),
        P(6, 1),
        P(6, 2),
        P(8, 1),
        P(1, 2)
    };
    const unsigned size = sizeof(array)/sizeof(array[0]);
    Iter r = std::stable_partition(Iter(array), Iter(array+size), odd_first());
    TC_ASSERT_EXPR(base(r) == array + 1);
    TC_ASSERT_EXPR(array[0] == P(1, 2));
    TC_ASSERT_EXPR(array[1] == P(0, 1));
    TC_ASSERT_EXPR(array[2] == P(0, 2));
    TC_ASSERT_EXPR(array[3] == P(2, 1));
    TC_ASSERT_EXPR(array[4] == P(2, 2));
    TC_ASSERT_EXPR(array[5] == P(4, 1));
    TC_ASSERT_EXPR(array[6] == P(4, 2));
    TC_ASSERT_EXPR(array[7] == P(6, 1));
    TC_ASSERT_EXPR(array[8] == P(6, 2));
    TC_ASSERT_EXPR(array[9] == P(8, 1));
  }
  { // check all true but first false
    typedef std::pair<int,int> P;
    P array[] =
    {
        P(0, 1),
        P(1, 2),
        P(3, 1),
        P(3, 2),
        P(5, 1),
        P(5, 2),
        P(7, 1),
        P(7, 2),
        P(9, 1),
        P(9, 2)
    };
    const unsigned size = sizeof(array)/sizeof(array[0]);
    Iter r = std::stable_partition(Iter(array), Iter(array+size), odd_first());
    TC_ASSERT_EXPR(base(r) == array + size-1);
    TC_ASSERT_EXPR(array[0] == P(1, 2));
    TC_ASSERT_EXPR(array[1] == P(3, 1));
    TC_ASSERT_EXPR(array[2] == P(3, 2));
    TC_ASSERT_EXPR(array[3] == P(5, 1));
    TC_ASSERT_EXPR(array[4] == P(5, 2));
    TC_ASSERT_EXPR(array[5] == P(7, 1));
    TC_ASSERT_EXPR(array[6] == P(7, 2));
    TC_ASSERT_EXPR(array[7] == P(9, 1));
    TC_ASSERT_EXPR(array[8] == P(9, 2));
    TC_ASSERT_EXPR(array[9] == P(0, 1));
  }
  { // check all true but last false
    typedef std::pair<int,int> P;
    P array[] =
    {
        P(1, 1),
        P(1, 2),
        P(3, 1),
        P(3, 2),
        P(5, 1),
        P(5, 2),
        P(7, 1),
        P(7, 2),
        P(9, 1),
        P(0, 2)
    };
    const unsigned size = sizeof(array)/sizeof(array[0]);
    Iter r = std::stable_partition(Iter(array), Iter(array+size), odd_first());
    TC_ASSERT_EXPR(base(r) == array + size-1);
    TC_ASSERT_EXPR(array[0] == P(1, 1));
    TC_ASSERT_EXPR(array[1] == P(1, 2));
    TC_ASSERT_EXPR(array[2] == P(3, 1));
    TC_ASSERT_EXPR(array[3] == P(3, 2));
    TC_ASSERT_EXPR(array[4] == P(5, 1));
    TC_ASSERT_EXPR(array[5] == P(5, 2));
    TC_ASSERT_EXPR(array[6] == P(7, 1));
    TC_ASSERT_EXPR(array[7] == P(7, 2));
    TC_ASSERT_EXPR(array[8] == P(9, 1));
    TC_ASSERT_EXPR(array[9] == P(0, 2));
  }
#if TEST_STD_VER >= 11 && !defined(TEST_HAS_NO_EXCEPTIONS)
  // TODO: Re-enable this test once we're no longer using get_temporary_buffer().
  // For now it trips up GCC due to the use of always_inline.
#if 0
  { // check that the algorithm still works when no memory is available
    std::vector<int> vec(150, 3);
    vec[5]                             = 6;
    getGlobalMemCounter()->throw_after = 0;
    std::stable_partition(vec.begin(), vec.end(), [](int i) { return i < 5; });
    TC_ASSERT_EXPR(std::is_partitioned(vec.begin(), vec.end(), [](int i) { return i < 5; }));
    vec[5]                             = 6;
    getGlobalMemCounter()->throw_after = 0;
    std::stable_partition(
        forward_iterator<int*>(vec.data()), forward_iterator<int*>(vec.data() + vec.size()), [](int i) {
          return i < 5;
        });
    TC_ASSERT_EXPR(std::is_partitioned(vec.begin(), vec.end(), [](int i) { return i < 5; }));
    getGlobalMemCounter()->reset();
  }
#endif
#endif // TEST_STD_VER >= 11 && !defined(TEST_HAS_NO_EXCEPTIONS)
}

#if TEST_STD_VER >= 11

struct is_null
{
    template <class P>
        bool operator()(const P& p) {return p == 0;}
};

template <class Iter>
void
test1()
{
    const unsigned size = 5;
    std::unique_ptr<int> array[size];
    Iter r = std::stable_partition(Iter(array), Iter(array+size), is_null());
    TC_ASSERT_EXPR(r == Iter(array+size));
}

#endif // TEST_STD_VER >= 11

} // namespace

int tc_libcxx_algorithms_alg_modifying_operations_alg_partitions_stable_partition(void) {
    test<bidirectional_iterator<std::pair<int,int>*> >();
    test<random_access_iterator<std::pair<int,int>*> >();
    test<std::pair<int,int>*>();

#if TEST_STD_VER >= 11
    test1<bidirectional_iterator<std::unique_ptr<int>*> >();
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
