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
// UNSUPPORTED: c++03, c++11, c++14

// <algorithm>

// template <class PopulationIterator, class SampleIterator, class Distance,
//           class UniformRandomNumberGenerator>
// SampleIterator sample(PopulationIterator first, PopulationIterator last,
//                       SampleIterator out, Distance n,
//                       UniformRandomNumberGenerator &&g);

#include <algorithm>
#include <random>
#include <type_traits>
#include <cassert>
#include <cstddef>

#include "test_iterators.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

namespace {
struct ReservoirSampleExpectations {
  enum { os = 4 };
  static int oa1[os];
  static int oa2[os];
};

int ReservoirSampleExpectations::oa1[] = {10, 5, 9, 4};
int ReservoirSampleExpectations::oa2[] = {5, 2, 10, 4};

struct SelectionSampleExpectations {
  enum { os = 4 };
  static int oa1[os];
  static int oa2[os];
};

int SelectionSampleExpectations::oa1[] = {1, 4, 6, 7};
int SelectionSampleExpectations::oa2[] = {1, 2, 6, 8};

template <class IteratorCategory> struct TestExpectations
    : public SelectionSampleExpectations {};

template <>
struct TestExpectations<std::input_iterator_tag>
    : public ReservoirSampleExpectations {};

template <template<class...> class PopulationIteratorType, class PopulationItem,
          template<class...> class SampleIteratorType, class SampleItem>
void test() {
  typedef PopulationIteratorType<PopulationItem *> PopulationIterator;
  typedef SampleIteratorType<SampleItem *> SampleIterator;
  PopulationItem ia[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  const unsigned is = sizeof(ia) / sizeof(ia[0]);
  typedef TestExpectations<typename std::iterator_traits<
      PopulationIterator>::iterator_category> Expectations;
  const unsigned os = Expectations::os;
  SampleItem oa[os];
  const int *oa1 = Expectations::oa1;
  ((void)oa1); // Prevent unused warning
  const int *oa2 = Expectations::oa2;
  ((void)oa2); // Prevent unused warning
  std::minstd_rand g;
  SampleIterator end = std::sample(PopulationIterator(ia),
                                   PopulationIterator(ia + is),
                                   SampleIterator(oa), os, g);
  TC_ASSERT_EXPR(static_cast<std::size_t>(base(end) - oa) == std::min(os, is));
  // sample() is deterministic but non-reproducible;
  // its results can vary between implementations.
  LIBCPP_ASSERT(std::equal(oa, oa + os, oa1));
  end = std::sample(PopulationIterator(ia),
                                  PopulationIterator(ia + is),
                                  SampleIterator(oa), os, std::move(g));
  TC_ASSERT_EXPR(static_cast<std::size_t>(base(end) - oa) == std::min(os, is));
  LIBCPP_ASSERT(std::equal(oa, oa + os, oa2));
}

template <template<class...> class PopulationIteratorType, class PopulationItem,
          template<class...> class SampleIteratorType, class SampleItem>
void test_empty_population() {
  typedef PopulationIteratorType<PopulationItem *> PopulationIterator;
  typedef SampleIteratorType<SampleItem *> SampleIterator;
  PopulationItem ia[] = {42};
  const unsigned os = 4;
  SampleItem oa[os];
  std::minstd_rand g;
  SampleIterator end =
      std::sample(PopulationIterator(ia), PopulationIterator(ia),
                                SampleIterator(oa), os, g);
  TC_ASSERT_EXPR(base(end) == oa);
}

template <template<class...> class PopulationIteratorType, class PopulationItem,
          template<class...> class SampleIteratorType, class SampleItem>
void test_empty_sample() {
  typedef PopulationIteratorType<PopulationItem *> PopulationIterator;
  typedef SampleIteratorType<SampleItem *> SampleIterator;
  PopulationItem ia[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  const unsigned is = sizeof(ia) / sizeof(ia[0]);
  SampleItem oa[1];
  std::minstd_rand g;
  SampleIterator end =
      std::sample(PopulationIterator(ia), PopulationIterator(ia + is),
                                SampleIterator(oa), 0, g);
  TC_ASSERT_EXPR(base(end) == oa);
}

template <template<class...> class PopulationIteratorType, class PopulationItem,
          template<class...> class SampleIteratorType, class SampleItem>
void test_small_population() {
  // The population size is less than the sample size.
  typedef PopulationIteratorType<PopulationItem *> PopulationIterator;
  typedef SampleIteratorType<SampleItem *> SampleIterator;
  PopulationItem ia[] = {1, 2, 3, 4, 5};
  const unsigned is = sizeof(ia) / sizeof(ia[0]);
  const unsigned os = 8;
  SampleItem oa[os];
  const SampleItem oa1[] = {1, 2, 3, 4, 5};
  std::minstd_rand g;
  SampleIterator end = std::sample(PopulationIterator(ia),
                                   PopulationIterator(ia + is),
                                   SampleIterator(oa), os, g);
  TC_ASSERT_EXPR(static_cast<std::size_t>(base(end) - oa) == std::min(os, is));
  typedef typename std::iterator_traits<PopulationIterator>::iterator_category PopulationCategory;
  if (std::is_base_of<std::forward_iterator_tag, PopulationCategory>::value) {
    TC_ASSERT_EXPR(std::equal(oa, base(end), oa1));
  } else {
    TC_ASSERT_EXPR(std::is_permutation(oa, base(end), oa1));
  }
}

} // namespace

int tc_libcxx_algorithms_alg_modifying_operations_alg_random_sample_sample(void) {
  test<cpp17_input_iterator, int, random_access_iterator, int>();
  test<forward_iterator, int, cpp17_output_iterator, int>();
  test<forward_iterator, int, random_access_iterator, int>();

  test<cpp17_input_iterator, int, random_access_iterator, double>();
  test<forward_iterator, int, cpp17_output_iterator, double>();
  test<forward_iterator, int, random_access_iterator, double>();

  test_empty_population<cpp17_input_iterator, int, random_access_iterator, int>();
  test_empty_population<forward_iterator, int, cpp17_output_iterator, int>();
  test_empty_population<forward_iterator, int, random_access_iterator, int>();

  test_empty_sample<cpp17_input_iterator, int, random_access_iterator, int>();
  test_empty_sample<forward_iterator, int, cpp17_output_iterator, int>();
  test_empty_sample<forward_iterator, int, random_access_iterator, int>();

  test_small_population<cpp17_input_iterator, int, random_access_iterator, int>();
  test_small_population<forward_iterator, int, cpp17_output_iterator, int>();
  test_small_population<forward_iterator, int, random_access_iterator, int>();

  TC_SUCCESS_RESULT();


  return 0;
}
