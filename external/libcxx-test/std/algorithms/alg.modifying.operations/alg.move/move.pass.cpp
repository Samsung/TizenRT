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
// UNSUPPORTED: c++03 && !stdlib=libc++
// ADDITIONAL_COMPILE_FLAGS(has-fconstexpr-steps): -fconstexpr-steps=2000000

// <algorithm>

// template<InputIterator InIter, typename OutIter>
//   requires OutputIterator<OutIter, RvalueOf<InIter::reference>::type>
//   OutIter
//   move(InIter first, InIter last, OutIter result);

#include <algorithm>
#include <cassert>
#include <iterator>
#include <memory>

#include "MoveOnly.h"
#include "test_iterators.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace {
class PaddedBase {
public:
  TEST_CONSTEXPR PaddedBase(std::int16_t a, std::int8_t b) : a_(a), b_(b) {}

  std::int16_t a_;
  std::int8_t b_;
};

class Derived : public PaddedBase {
public:
  TEST_CONSTEXPR Derived(std::int16_t a, std::int8_t b, std::int8_t c) : PaddedBase(a, b), c_(c) {}

  std::int8_t c_;
};

template <class InIter>
struct Test {
  template <class OutIter>
  TEST_CONSTEXPR_CXX20 void operator()() {
    const unsigned N = 1000;
    int ia[N] = {};
    for (unsigned i = 0; i < N; ++i)
        ia[i] = i;
    int ib[N] = {0};

    OutIter r = std::move(InIter(ia), InIter(ia+N), OutIter(ib));
    TC_ASSERT_EXPR(base(r) == ib+N);
    for (unsigned i = 0; i < N; ++i)
        TC_ASSERT_EXPR(ia[i] == ib[i]);
  }
};

struct TestOutIters {
  template <class InIter>
  TEST_CONSTEXPR_CXX20 void operator()() {
    types::for_each(
        types::concatenate_t<types::cpp17_input_iterator_list<int*>, types::type_list<cpp17_output_iterator<int*> > >(),
        Test<InIter>());
  }
};

template <class InIter>
struct Test1 {
  template <class OutIter>
  TEST_CONSTEXPR_CXX23 void operator()() {
    const unsigned N = 100;
    std::unique_ptr<int> ia[N];
    for (unsigned i = 0; i < N; ++i)
        ia[i].reset(new int(i));
    std::unique_ptr<int> ib[N];

    OutIter r = std::move(InIter(ia), InIter(ia+N), OutIter(ib));
    TC_ASSERT_EXPR(base(r) == ib+N);
    for (unsigned i = 0; i < N; ++i)
        TC_ASSERT_EXPR(*ib[i] == static_cast<int>(i));
  }
};

struct Test1OutIters {
  template <class InIter>
  TEST_CONSTEXPR_CXX23 void operator()() {
    types::for_each(types::concatenate_t<types::cpp17_input_iterator_list<std::unique_ptr<int>*>,
                                         types::type_list<cpp17_output_iterator<std::unique_ptr<int>*> > >(),
                    Test1<InIter>());
  }
};

TEST_CONSTEXPR_CXX20 bool test() {
  types::for_each(types::cpp17_input_iterator_list<int*>(), TestOutIters());
  if (TEST_STD_VER >= 23 || !TEST_IS_CONSTANT_EVALUATED)
    types::for_each(types::cpp17_input_iterator_list<std::unique_ptr<int>*>(), Test1OutIters());

  { // Make sure that padding bits aren't copied
    Derived src(1, 2, 3);
    Derived dst(4, 5, 6);
    std::move(static_cast<PaddedBase*>(&src), static_cast<PaddedBase*>(&src) + 1, static_cast<PaddedBase*>(&dst));
    TC_ASSERT_EXPR(dst.a_ == 1);
    TC_ASSERT_EXPR(dst.b_ == 2);
    TC_ASSERT_EXPR(dst.c_ == 6);
  }

  { // Make sure that overlapping ranges can be copied
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::move(a + 3, a + 10, a);
    int expected[] = {4, 5, 6, 7, 8, 9, 10, 8, 9, 10};
    TC_ASSERT_EXPR(std::equal(a, a + 10, expected));
  }

  // Make sure that the algorithm works with move-only types
  {
    // When non-trivial
    {
      MoveOnly from[3] = {1, 2, 3};
      MoveOnly to[3] = {};
      std::move(std::begin(from), std::end(from), std::begin(to));
      TC_ASSERT_EXPR(to[0] == MoveOnly(1));
      TC_ASSERT_EXPR(to[1] == MoveOnly(2));
      TC_ASSERT_EXPR(to[2] == MoveOnly(3));
    }
    // When trivial
    {
      TrivialMoveOnly from[3] = {1, 2, 3};
      TrivialMoveOnly to[3] = {};
      std::move(std::begin(from), std::end(from), std::begin(to));
      TC_ASSERT_EXPR(to[0] == TrivialMoveOnly(1));
      TC_ASSERT_EXPR(to[1] == TrivialMoveOnly(2));
      TC_ASSERT_EXPR(to[2] == TrivialMoveOnly(3));
    }
  }

  return true;
}

} // namespace

int tc_libcxx_algorithms_alg_modifying_operations_alg_move_move(void) {
  test();
#if TEST_STD_VER >= 20
  static_assert(test());
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
