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

// template<class ExecutionPolicy, class ForwardIterator1, class ForwardIterator2,
//          class Predicate, class T>
//   ForwardIterator2
//     replace_copy_if(ExecutionPolicy&& exec,
//                     ForwardIterator1 first, ForwardIterator1 last,
//                     ForwardIterator2 result,
//                     Predicate pred, const T& new_value);

#include <algorithm>
#include <array>
#include <cassert>
#include <vector>

#include "type_algorithms.h"
#include "test_execution_policies.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

template <class Iter>
struct Test {
  template <class ExecutionPolicy>
  void operator()(ExecutionPolicy&& policy) {
    { // simple test
      std::array a = {1, 2, 3, 4, 5, 6, 7, 8};
      std::array<int, a.size()> out;
      std::replace_copy_if(
          policy, Iter(std::begin(a)), Iter(std::end(a)), Iter(std::begin(out)), [](int i) { return i == 3; }, 6);
      TC_ASSERT_EXPR((out == std::array{1, 2, 6, 4, 5, 6, 7, 8}));
    }

    { // empty range works
      std::array<int, 0> a = {};
      std::replace_copy_if(
          policy, Iter(std::begin(a)), Iter(std::end(a)), Iter(std::begin(a)), [](int i) { return i == 3; }, 6);
    }

    { // non-empty range without a match works
      std::array a = {1, 2};
      std::array<int, a.size()> out;
      std::replace_copy_if(
          policy, Iter(std::begin(a)), Iter(std::end(a)), Iter(out.data()), [](int i) { return i == 3; }, 6);
      TC_ASSERT_EXPR((out == std::array{1, 2}));
    }

    { // single element range works
      std::array a = {3};
      std::array<int, a.size()> out;
      std::replace_copy_if(
          policy, Iter(std::begin(a)), Iter(std::end(a)), Iter(std::begin(out)), [](int i) { return i == 3; }, 6);
      TC_ASSERT_EXPR((out == std::array{6}));
    }

    { // two element range works
      std::array a = {3, 4};
      std::array<int, a.size()> out;
      std::replace_copy_if(
          policy, Iter(std::begin(a)), Iter(std::end(a)), Iter(std::begin(out)), [](int i) { return i == 3; }, 6);
      TC_ASSERT_EXPR((out == std::array{6, 4}));
    }

    { // multiple matching elements work
      std::array a = {1, 2, 3, 4, 3, 3, 5, 6, 3};
      std::array<int, a.size()> out;
      std::replace_copy_if(
          policy, Iter(std::begin(a)), Iter(std::end(a)), Iter(std::begin(out)), [](int i) { return i == 3; }, 9);
      TC_ASSERT_EXPR((out == std::array{1, 2, 9, 4, 9, 9, 5, 6, 9}));
    }

    { // large range works
      std::vector<int> a(150, 3);
      std::vector<int> out(a.size());
      a[45] = 5;
      std::replace_copy_if(
          policy,
          Iter(std::data(a)),
          Iter(std::data(a) + std::size(a)),
          Iter(out.data()),
          [](int i) { return i == 3; },
          6);

      std::vector<int> comp(150, 6);
      comp[45] = 5;
      TC_ASSERT_EXPR(std::equal(out.begin(), out.end(), comp.begin()));
    }
  }
};

struct ThrowOnCompare {};

#ifndef TEST_HAS_NO_EXCEPTIONS
bool operator==(ThrowOnCompare, ThrowOnCompare) { throw int{}; }
#endif

int tc_algorithms_alg_modifying_operations_alg_replace_pstl_replace_copy_if(void) {
  types::for_each(types::forward_iterator_list<int*>{}, TestIteratorWithPolicies<Test>{});

#ifndef TEST_HAS_NO_EXCEPTIONS
  std::set_terminate(terminate_successful);
  ThrowOnCompare a[2];
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    (void)std::replace_copy_if(
        std::execution::par,
        std::begin(a),
        std::end(a),
        std::begin(a),
        [](ThrowOnCompare& i) { return i == i; },
        ThrowOnCompare{});
  } catch (int) {
    TC_ASSERT_EXPR(false);
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif

  return 0;
}
