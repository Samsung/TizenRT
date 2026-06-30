//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<InputIterator InIter, OutputIterator<auto, InIter::reference> OutIter>
//   constexpr OutIter   // constexpr after C++17
//   copy(InIter first, InIter last, OutIter result);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
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
  TEST_CONSTEXPR Derived(std::int16_t a, std::int8_t b, std::int32_t c) : PaddedBase(a, b), c_(c) {}

  std::int32_t c_;
};

template <class InIter>
struct Test {
  template <class OutIter>
  TEST_CONSTEXPR_CXX20 void operator()() {
    const unsigned N = TEST_ARRAY_SIZE;
    int ia[N]        = {};
    for (unsigned i = 0; i < N; ++i)
      ia[i] = i;
    int ib[N] = {0};

    OutIter r = std::copy(InIter(ia), InIter(ia + N), OutIter(ib));
    TC_ASSERT_EXPR(base(r) == ib + N);
    for (unsigned i = 0; i < N; ++i)
      TC_ASSERT_EXPR(ia[i] == ib[i]);
  }
};

struct TestInIters {
  template <class InIter>
  TEST_CONSTEXPR_CXX20 void operator()() {
    types::for_each(
        types::concatenate_t<types::cpp17_input_iterator_list<int*>, types::type_list<cpp17_output_iterator<int*> > >(),
        Test<InIter>());
  }
};

TEST_CONSTEXPR_CXX20 bool test() {
  types::for_each(types::cpp17_input_iterator_list<int*>(), TestInIters());

  { // Make sure that padding bits aren't copied
    Derived src(1, 2, 3);
    Derived dst(4, 5, 6);
    std::copy(static_cast<PaddedBase*>(&src), static_cast<PaddedBase*>(&src) + 1, static_cast<PaddedBase*>(&dst));
    TC_ASSERT_EXPR(dst.a_ == 1);
    TC_ASSERT_EXPR(dst.b_ == 2);
    TC_ASSERT_EXPR(dst.c_ == 6);
  }

  { // Make sure that overlapping ranges can be copied
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::copy(a + 3, a + 10, a);
    int expected[] = {4, 5, 6, 7, 8, 9, 10, 8, 9, 10};
    TC_ASSERT_EXPR(std::equal(a, a + 10, expected));
  }

  return true;
}

} // namespace

int tc_libcxx_algorithms_alg_modifying_operations_alg_copy_copy(void) 
{
  printf("\nStarting Runtime test\n");
  test();

#if TEST_STD_VER > 17
  printf("\nStarting static test\n");
  static_assert(test());
#endif

  TC_SUCCESS_RESULT();


  return 0;
}
