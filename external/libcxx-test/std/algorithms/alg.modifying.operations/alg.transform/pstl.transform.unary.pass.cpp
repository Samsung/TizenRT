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

// template<class ExecutionPolicy, class ForwardIterator1, class ForwardIterator2,
//          class UnaryOperation>
//   ForwardIterator2
//     transform(ExecutionPolicy&& exec,
//               ForwardIterator1 first1, ForwardIterator1 last1,
//               ForwardIterator2 result, UnaryOperation op);

#include <algorithm>
#include <cassert>
#include <vector>

#include "test_macros.h"
#include "test_execution_policies.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

// We can't test the constraint on the execution policy, because that would conflict with the binary
// transform algorithm that doesn't take an execution policy, which is not constrained at all.

template <class Iter1, class Iter2>
struct Test {
  template <class Policy>
  void operator()(Policy&& policy) {
    // simple test
    for (const int size : {0, 1, 2, 100, 350}) {
      std::vector<int> a(size);
      for (int i = 0; i != size; ++i)
        a[i] = i + 1;

      std::vector<int> out(std::size(a));
      decltype(auto) ret = std::transform(
          policy, Iter1(std::data(a)), Iter1(std::data(a) + std::size(a)), Iter2(std::data(out)), [](int i) {
            return i + 3;
          });
      static_assert(std::is_same_v<decltype(ret), Iter2>);
      TC_ASSERT_EXPR(base(ret) == std::data(out) + std::size(out));
      for (int i = 0; i != size; ++i)
        TC_ASSERT_EXPR(out[i] == i + 4);
    }
  }
};

int tc_algorithms_alg_modifying_operations_alg_transform_pstl_transform_unary(void) {
  types::for_each(types::forward_iterator_list<int*>{}, types::apply_type_identity{[](auto v) {
                    using Iter = typename decltype(v)::type;
                    types::for_each(
                        types::forward_iterator_list<int*>{},
                        TestIteratorWithPolicies<types::partial_instantiation<Test, Iter>::template apply>{});
                  }});

  return 0;
}
