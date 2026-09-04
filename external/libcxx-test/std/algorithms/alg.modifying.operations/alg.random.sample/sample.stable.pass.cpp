//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// UNSUPPORTED: c++03, c++11, c++14

// <algorithm>

// template <class PopulationIterator, class SampleIterator, class Distance,
//           class UniformRandomNumberGenerator>
// SampleIterator sample(PopulationIterator first, PopulationIterator last,
//                       SampleIterator out, Distance n,
//                       UniformRandomNumberGenerator &&g);

#include <algorithm>
#include <random>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

namespace {
// Stable if and only if PopulationIterator meets the requirements of a
// ForwardIterator type.
template <class PopulationIterator, class SampleIterator>
void test_stability(bool expect_stable) {
  const unsigned kPopulationSize = 100;
  int ia[kPopulationSize];
  for (unsigned i = 0; i < kPopulationSize; ++i)
    ia[i] = i;
  PopulationIterator first(ia);
  PopulationIterator last(ia + kPopulationSize);

  const unsigned kSampleSize = 20;
  int oa[kPopulationSize];
  SampleIterator out(oa);

  std::minstd_rand g;

  const int kIterations = 1000;
  bool unstable = false;
  for (int i = 0; i < kIterations; ++i) {
    std::sample(first, last, out, kSampleSize, g);
    unstable |= !std::is_sorted(oa, oa + kSampleSize);
  }
  TC_ASSERT_EXPR(expect_stable == !unstable);
}

} // namespace

int tc_libcxx_algorithms_alg_modifying_operations_alg_random_sample_sample_stable(void) {
  test_stability<forward_iterator<int *>, cpp17_output_iterator<int *> >(true);
  test_stability<cpp17_input_iterator<int *>, random_access_iterator<int *> >(false);

  TC_SUCCESS_RESULT();


  return 0;
}
