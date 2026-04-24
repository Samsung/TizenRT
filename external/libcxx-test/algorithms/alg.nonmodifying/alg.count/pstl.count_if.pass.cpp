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
//   typename iterator_traits<ForwardIterator>::difference_type
//     count_if(ExecutionPolicy&& exec,
//              ForwardIterator first, ForwardIterator last, Predicate pred);

#include <algorithm>
#include <array>
#include <cassert>
#include <vector>

#include "test_macros.h"
#include "test_execution_policies.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

EXECUTION_POLICY_SFINAE_TEST(count_if);

static_assert(sfinae_test_count_if<int, int*, int*, bool (*)(int)>);
static_assert(!sfinae_test_count_if<std::execution::parallel_policy, int*, int*, int>);

template <class Iter>
struct Test {
  template <class Policy>
  void operator()(Policy&& policy) {
    { // simple test
      int a[]            = {1, 2, 3, 4, 5};
      decltype(auto) ret = std::count_if(policy, std::begin(a), std::end(a), [](int i) { return i < 3; });
      static_assert(std::is_same_v<decltype(ret), typename std::iterator_traits<Iter>::difference_type>);
      TC_ASSERT_EXPR(ret == 2);
    }

    { // test that an empty range works
      std::array<int, 0> a;
      decltype(auto) ret = std::count_if(policy, std::begin(a), std::end(a), [](int i) { return i < 3; });
      static_assert(std::is_same_v<decltype(ret), typename std::iterator_traits<Iter>::difference_type>);
      TC_ASSERT_EXPR(ret == 0);
    }

    { // test that a single-element range works
      int a[] = {1};
      decltype(auto) ret = std::count_if(policy, std::begin(a), std::end(a), [](int i) { return i < 3; });
      static_assert(std::is_same_v<decltype(ret), typename std::iterator_traits<Iter>::difference_type>);
      TC_ASSERT_EXPR(ret == 1);
    }

    { // test that a two-element range works
      int a[] = {1, 3};
      decltype(auto) ret = std::count_if(policy, std::begin(a), std::end(a), [](int i) { return i < 3; });
      static_assert(std::is_same_v<decltype(ret), typename std::iterator_traits<Iter>::difference_type>);
      TC_ASSERT_EXPR(ret == 1);
    }

    { // test that a three-element range works
      int a[] = {2, 3, 2};
      decltype(auto) ret = std::count_if(policy, std::begin(a), std::end(a), [](int i) { return i < 3; });
      static_assert(std::is_same_v<decltype(ret), typename std::iterator_traits<Iter>::difference_type>);
      TC_ASSERT_EXPR(ret == 2);
    }

    { // test that a large range works
      std::vector<int> a(100, 2);
      decltype(auto) ret = std::count_if(policy, std::begin(a), std::end(a), [](int i) { return i < 3; });
      static_assert(std::is_same_v<decltype(ret), typename std::iterator_traits<Iter>::difference_type>);
      TC_ASSERT_EXPR(ret == 100);
    }
  }
};

int tc_algorithms_alg_nonmodifying_alg_count_pstl_count_if(void) {
  types::for_each(types::forward_iterator_list<int*>{}, TestIteratorWithPolicies<Test>{});

  return 0;
}
