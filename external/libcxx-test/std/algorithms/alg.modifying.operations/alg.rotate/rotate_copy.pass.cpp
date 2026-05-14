//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// <algorithm>

// template<ForwardIterator InIter, OutputIterator<auto, InIter::reference> OutIter>
//   constexpr OutIter          // constexpr after C++17
//   rotate_copy(InIter first, InIter middle, InIter last, OutIter result);

#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"


namespace {
template <class InIter, class OutIter>
TEST_CONSTEXPR_CXX20 void test() {
  int ia[] = {0, 1, 2, 3};
  const unsigned sa = sizeof(ia) / sizeof(ia[0]);
  int ib[sa] = {0};

  OutIter r = std::rotate_copy(InIter(ia), InIter(ia), InIter(ia), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib);

  r = std::rotate_copy(InIter(ia), InIter(ia), InIter(ia + 1), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 1);
  TC_ASSERT_EXPR(ib[0] == 0);

  r = std::rotate_copy(InIter(ia), InIter(ia + 1), InIter(ia + 1), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 1);
  TC_ASSERT_EXPR(ib[0] == 0);

  r = std::rotate_copy(InIter(ia), InIter(ia), InIter(ia + 2), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 2);
  TC_ASSERT_EXPR(ib[0] == 0);
  TC_ASSERT_EXPR(ib[1] == 1);

  r = std::rotate_copy(InIter(ia), InIter(ia + 1), InIter(ia + 2), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 2);
  TC_ASSERT_EXPR(ib[0] == 1);
  TC_ASSERT_EXPR(ib[1] == 0);

  r = std::rotate_copy(InIter(ia), InIter(ia + 2), InIter(ia + 2), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 2);
  TC_ASSERT_EXPR(ib[0] == 0);
  TC_ASSERT_EXPR(ib[1] == 1);

  r = std::rotate_copy(InIter(ia), InIter(ia), InIter(ia + 3), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 3);
  TC_ASSERT_EXPR(ib[0] == 0);
  TC_ASSERT_EXPR(ib[1] == 1);
  TC_ASSERT_EXPR(ib[2] == 2);

  r = std::rotate_copy(InIter(ia), InIter(ia + 1), InIter(ia + 3), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 3);
  TC_ASSERT_EXPR(ib[0] == 1);
  TC_ASSERT_EXPR(ib[1] == 2);
  TC_ASSERT_EXPR(ib[2] == 0);

  r = std::rotate_copy(InIter(ia), InIter(ia + 2), InIter(ia + 3), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 3);
  TC_ASSERT_EXPR(ib[0] == 2);
  TC_ASSERT_EXPR(ib[1] == 0);
  TC_ASSERT_EXPR(ib[2] == 1);

  r = std::rotate_copy(InIter(ia), InIter(ia + 3), InIter(ia + 3), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 3);
  TC_ASSERT_EXPR(ib[0] == 0);
  TC_ASSERT_EXPR(ib[1] == 1);
  TC_ASSERT_EXPR(ib[2] == 2);

  r = std::rotate_copy(InIter(ia), InIter(ia), InIter(ia + 4), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 4);
  TC_ASSERT_EXPR(ib[0] == 0);
  TC_ASSERT_EXPR(ib[1] == 1);
  TC_ASSERT_EXPR(ib[2] == 2);
  TC_ASSERT_EXPR(ib[3] == 3);

  r = std::rotate_copy(InIter(ia), InIter(ia + 1), InIter(ia + 4), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 4);
  TC_ASSERT_EXPR(ib[0] == 1);
  TC_ASSERT_EXPR(ib[1] == 2);
  TC_ASSERT_EXPR(ib[2] == 3);
  TC_ASSERT_EXPR(ib[3] == 0);

  r = std::rotate_copy(InIter(ia), InIter(ia + 2), InIter(ia + 4), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 4);
  TC_ASSERT_EXPR(ib[0] == 2);
  TC_ASSERT_EXPR(ib[1] == 3);
  TC_ASSERT_EXPR(ib[2] == 0);
  TC_ASSERT_EXPR(ib[3] == 1);

  r = std::rotate_copy(InIter(ia), InIter(ia + 3), InIter(ia + 4), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 4);
  TC_ASSERT_EXPR(ib[0] == 3);
  TC_ASSERT_EXPR(ib[1] == 0);
  TC_ASSERT_EXPR(ib[2] == 1);
  TC_ASSERT_EXPR(ib[3] == 2);

  r = std::rotate_copy(InIter(ia), InIter(ia + 4), InIter(ia + 4), OutIter(ib));
  TC_ASSERT_EXPR(base(r) == ib + 4);
  TC_ASSERT_EXPR(ib[0] == 0);
  TC_ASSERT_EXPR(ib[1] == 1);
  TC_ASSERT_EXPR(ib[2] == 2);
  TC_ASSERT_EXPR(ib[3] == 3);

  {
    int ints[] = {1, 3, 5, 2, 5, 6};
    int const n_ints = sizeof(ints)/sizeof(int);
    int zeros[n_ints] = {0};

    const std::size_t N = 2;
    const auto middle = std::begin(ints) + N;
    auto it = std::rotate_copy(std::begin(ints), middle, std::end(ints), std::begin(zeros));
    TC_ASSERT_EXPR(std::distance(std::begin(zeros), it) == n_ints);
    TC_ASSERT_EXPR(std::equal(std::begin(ints), middle, std::begin(zeros) + n_ints - N));
    TC_ASSERT_EXPR(std::equal(middle, std::end(ints), std::begin(zeros)));
  }
}

TEST_CONSTEXPR_CXX20 bool all_tests() {
  test<bidirectional_iterator<const int*>, cpp17_output_iterator<int*> >();
  test<bidirectional_iterator<const int*>, forward_iterator<int*> >();
  test<bidirectional_iterator<const int*>, bidirectional_iterator<int*> >();
  test<bidirectional_iterator<const int*>, random_access_iterator<int*> >();
  test<bidirectional_iterator<const int*>, int*>();

  test<random_access_iterator<const int*>, cpp17_output_iterator<int*> >();
  test<random_access_iterator<const int*>, forward_iterator<int*> >();
  test<random_access_iterator<const int*>, bidirectional_iterator<int*> >();
  test<random_access_iterator<const int*>, random_access_iterator<int*> >();
  test<random_access_iterator<const int*>, int*>();

  test<const int*, cpp17_output_iterator<int*> >();
  test<const int*, forward_iterator<int*> >();
  test<const int*, bidirectional_iterator<int*> >();
  test<const int*, random_access_iterator<int*> >();
  test<const int*, int*>();

  return true;
}

} // namespace

int tc_libcxx_algorithms_alg_modifying_operations_alg_rotate_rotate_copy(void) {
  all_tests();

#if TEST_STD_VER > 17
  static_assert(all_tests());
#endif
  TC_SUCCESS_RESULT();

  return 0;
}
