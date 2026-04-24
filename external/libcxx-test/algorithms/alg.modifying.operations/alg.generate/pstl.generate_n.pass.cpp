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

// template<class ExecutionPolicy, class ForwardIterator, class Size, class Generator>
//   ForwardIterator generate_n(ExecutionPolicy&& exec,
//                              ForwardIterator first, Size n, Generator gen);

#include <algorithm>
#include <cassert>
#include <vector>

#include "test_iterators.h"
#include "test_execution_policies.h"
#include "type_algorithms.h"
#include "libcxx_tc_common.h"

template <class Iter>
struct Test {
  template <class ExecutionPolicy>
  void operator()(ExecutionPolicy&& policy) {
    { // simple test
      int a[10];
      std::generate_n(policy, Iter(std::begin(a)), std::size(a), []() { return 1; });
      TC_ASSERT_EXPR(std::all_of(std::begin(a), std::end(a), [](int i) { return i == 1; }));
    }
    { // empty range works
      int a[10] {3};
      std::generate_n(policy, Iter(std::begin(a)), 0, []() { return 1; });
      TC_ASSERT_EXPR(a[0] == 3);
    }
    { // single-element range works
      int a[] {3};
      std::generate_n(policy, Iter(std::begin(a)), std::size(a), []() { return 5; });
      TC_ASSERT_EXPR(a[0] == 5);
    }
    { // large range works
      std::vector<int> vec(150, 4);
      std::generate_n(policy, Iter(std::data(vec)), std::size(vec), []() { return 5; });
      TC_ASSERT_EXPR(std::all_of(std::begin(vec), std::end(vec), [](int i) { return i == 5; }));
    }
  }
};

int tc_algorithms_alg_modifying_operations_alg_generate_pstl_generate_n(void) {
  types::for_each(types::forward_iterator_list<int*>{}, TestIteratorWithPolicies<Test>{});

  return 0;
}
