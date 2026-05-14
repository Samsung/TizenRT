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

// UNSUPPORTED: libcpp-has-no-incomplete-pstl

// <algorithm>

// template<class ExecutionPolicy, class ForwardIterator, class Predicate>
//   bool none_of(ExecutionPolicy&& exec, ForwardIterator first, ForwardIterator last,
//                Predicate pred);

#include <algorithm>
#include <cassert>
#include <vector>

#include "test_macros.h"
#include "test_execution_policies.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

EXECUTION_POLICY_SFINAE_TEST(none_of);

static_assert(sfinae_test_none_of<int, int*, int*, bool (*)(int)>);
static_assert(!sfinae_test_none_of<std::execution::parallel_policy, int*, int*, bool (*)(int)>);

template <class Iter>
struct Test {
  template <class Policy>
  void operator()(Policy&& policy) {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
    // simple test
    TC_ASSERT_EXPR(std::none_of(policy, Iter(std::begin(a)), Iter(std::end(a)), [](int i) { return i > 9; }));
    TC_ASSERT_EXPR(!std::none_of(policy, Iter(std::begin(a)), Iter(std::end(a)), [](int i) { return i >= 8; }));

    // check that an empty range works
    TC_ASSERT_EXPR(std::none_of(policy, Iter(std::begin(a)), Iter(std::begin(a)), [](int) { return false; }));

    // check that true is returned if no element satisfies the condition
    TC_ASSERT_EXPR(std::none_of(policy, Iter(std::begin(a)), Iter(std::end(a)), [](int i) { return i == 9; }));

    // check that false is returned if only one elements satisfies the condition
    TC_ASSERT_EXPR(!std::none_of(policy, Iter(std::begin(a)), Iter(std::end(a)), [](int i) { return i == 1; }));

    // check that a one-element range works
    TC_ASSERT_EXPR(std::none_of(policy, Iter(std::begin(a)), Iter(std::begin(a) + 1), [](int i) { return i != 1; }));

    // check that a two-element range works
    TC_ASSERT_EXPR(std::none_of(policy, Iter(std::begin(a)), Iter(std::begin(a) + 2), [](int i) { return i > 2; }));

    // check that a large number of elements works
    std::vector<int> vec(100);
    std::fill(vec.begin(), vec.end(), 3);
    TC_ASSERT_EXPR(std::none_of(Iter(vec.data()), Iter(vec.data() + vec.size()), [](int i) { return i != 3; }));
  }
};

int tc_algorithms_alg_nonmodifying_alg_none_of_pstl_none_of(void) {
  types::for_each(types::forward_iterator_list<int*>{}, TestIteratorWithPolicies<Test>{});

#ifndef TEST_HAS_NO_EXCEPTIONS
  std::set_terminate(terminate_successful);
  int a[] = {1, 2};
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    (void)std::none_of(std::execution::par, std::begin(a), std::end(a), [](int i) -> bool { throw i; });
  } catch (int) {
    TC_ASSERT_EXPR(false);
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif

  return 0;
}
