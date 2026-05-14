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

// template<class ExecutionPolicy, class ForwardIterator, class Function>
//   void for_each(ExecutionPolicy&& exec,
//                 ForwardIterator first, ForwardIterator last,
//                 Function f);

#include <algorithm>
#include <atomic>
#include <cassert>
#include <vector>

#include "test_macros.h"
#include "test_execution_policies.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

EXECUTION_POLICY_SFINAE_TEST(for_each);

static_assert(sfinae_test_for_each<int, int*, int*, bool (*)(int)>);
static_assert(!sfinae_test_for_each<std::execution::parallel_policy, int*, int*, bool (*)(int)>);

template <class Iter>
struct Test {
  template <class Policy>
  void operator()(Policy&& policy) {
    int sizes[] = {0, 1, 2, 100};
    for (auto size : sizes) {
      std::vector<int> a(size);
      std::vector<Bool> called(size);
      std::for_each(policy, Iter(std::data(a)), Iter(std::data(a) + std::size(a)), [&](int& v) {
        TC_ASSERT_EXPR(!called[&v - a.data()]);
        called[&v - a.data()] = true;
      });
      TC_ASSERT_EXPR(std::all_of(std::begin(called), std::end(called), [](bool b) { return b; }));
    }
  }
};

int tc_algorithms_alg_nonmodifying_alg_foreach_pstl_for_each(void) {
  types::for_each(types::forward_iterator_list<int*>{}, TestIteratorWithPolicies<Test>{});

#ifndef TEST_HAS_NO_EXCEPTIONS
  std::set_terminate(terminate_successful);
  int a[] = {1, 2};
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    std::for_each(std::execution::par, std::begin(a), std::end(a), [](int) { throw int{}; });
  } catch (int) {
    TC_ASSERT_EXPR(false);
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif

  return 0;
}
