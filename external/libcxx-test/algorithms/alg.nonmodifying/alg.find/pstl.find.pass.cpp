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

// template<class ExecutionPolicy, class ForwardIterator, class T>
//   ForwardIterator find(ExecutionPolicy&& exec, ForwardIterator first, ForwardIterator last,
//                        const T& value);

#include <algorithm>
#include <cassert>
#include <vector>

#include "test_macros.h"
#include "test_execution_policies.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

EXECUTION_POLICY_SFINAE_TEST(find);

static_assert(sfinae_test_find<int, int*, int*, bool (*)(int)>);
static_assert(!sfinae_test_find<std::execution::parallel_policy, int*, int*, int>);

template <class Iter>
struct Test {
  template <class Policy>
  void operator()(Policy&& policy) {
    int a[] = {1, 2, 3, 4, 5, 6, 7, 8};

    // simple test
    TC_ASSERT_EXPR(base(std::find(policy, Iter(std::begin(a)), Iter(std::end(a)), 3)) == a + 2);

    // check that last is returned if no element matches
    TC_ASSERT_EXPR(base(std::find(policy, Iter(std::begin(a)), Iter(std::end(a)), 0)) == std::end(a));

    // check that the first element is returned
    TC_ASSERT_EXPR(base(std::find(policy, Iter(std::begin(a)), Iter(std::end(a)), 1)) == std::begin(a));

    // check that an empty range works
    TC_ASSERT_EXPR(base(std::find(policy, Iter(std::begin(a)), Iter(std::begin(a)), 1)) == std::begin(a));

    // check that a one-element range works
    TC_ASSERT_EXPR(base(std::find(policy, Iter(std::begin(a)), Iter(std::begin(a) + 1), 1)) == std::begin(a));

    // check that a two-element range works
    TC_ASSERT_EXPR(base(std::find(policy, Iter(std::begin(a)), Iter(std::begin(a) + 2), 2)) == std::begin(a) + 1);

    // check that a large number of elements works
    std::vector<int> vec(200, 4);
    vec[176] = 5;
    TC_ASSERT_EXPR(base(std::find(policy, Iter(std::data(vec)), Iter(std::data(vec) + std::size(vec)), 5)) ==
           std::data(vec) + 176);
  }
};

struct ThrowOnCompare {};

#ifndef TEST_HAS_NO_EXCEPTIONS
bool operator==(ThrowOnCompare, ThrowOnCompare) { throw int{}; }
#endif

int tc_algorithms_alg_nonmodifying_alg_find_pstl_find(void) {
  types::for_each(types::forward_iterator_list<int*>{}, TestIteratorWithPolicies<Test>{});

#ifndef TEST_HAS_NO_EXCEPTIONS
  std::set_terminate(terminate_successful);
  ThrowOnCompare a[2];
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    (void)std::find(std::execution::par, std::begin(a), std::end(a), ThrowOnCompare{});
  } catch (int) {
    TC_ASSERT_EXPR(false);
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif

  return 0;
}
