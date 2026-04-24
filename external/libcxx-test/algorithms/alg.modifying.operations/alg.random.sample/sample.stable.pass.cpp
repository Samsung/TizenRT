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

int tc_algorithms_alg_modifying_operations_alg_random_sample_sample_stable(void) {
  test_stability<forward_iterator<int *>, cpp17_output_iterator<int *> >(true);
  test_stability<cpp17_input_iterator<int *>, random_access_iterator<int *> >(false);

  return 0;
}
