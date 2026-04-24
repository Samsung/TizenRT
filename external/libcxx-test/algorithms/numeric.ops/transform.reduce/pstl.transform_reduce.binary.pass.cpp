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

// template<class ExecutionPolicy,
//          class ForwardIterator1, class ForwardIterator2, class T>
//   T transform_reduce(ExecutionPolicy&& exec,
//                      ForwardIterator1 first1, ForwardIterator1 last1,
//                      ForwardIterator2 first2,
//                      T init);
//
// template<class ExecutionPolicy,
//          class ForwardIterator1, class ForwardIterator2, class T,
//          class BinaryOperation1, class BinaryOperation2>
//   T transform_reduce(ExecutionPolicy&& exec,
//                      ForwardIterator1 first1, ForwardIterator1 last1,
//                      ForwardIterator2 first2,
//                      T init,
//                      BinaryOperation1 binary_op1,
//                      BinaryOperation2 binary_op2);

#include <numeric>
#include <vector>

#include "MoveOnly.h"
#include "test_execution_policies.h"
#include "test_iterators.h"
#include "test_macros.h"
#include "type_algorithms.h"
#include "libcxx_tc_common.h"

template <class T>
struct constructible_from {
  T v_;

  explicit constructible_from(T v) : v_(v) {}

  friend constructible_from operator+(constructible_from lhs, constructible_from rhs) {
    return constructible_from{lhs.get() + rhs.get()};
  }

  T get() const { return v_; }
};

template <class Iter1, class Iter2, class ValueT>
struct Test {
  template <class Policy>
  void operator()(Policy&& policy) {
    for (const auto& pair : {std::pair{0, 34}, {1, 40}, {2, 48}, {100, 10534}, {350, 124284}}) {
      auto [size, expected] = pair;
      std::vector<int> a(size);
      std::vector<int> b(size);
      for (int i = 0; i != size; ++i) {
        a[i] = i + 1;
        b[i] = i + 4;
      }

      decltype(auto) ret = std::transform_reduce(
          policy,
          Iter1(std::data(a)),
          Iter1(std::data(a) + std::size(a)),
          Iter2(std::data(b)),
          ValueT(34),
          std::plus{},
          [](ValueT i, ValueT j) { return i + j + 1; });
      static_assert(std::is_same_v<decltype(ret), ValueT>);
      TC_ASSERT_EXPR(ret == expected);
    }

    for (const auto& pair : {std::pair{0, 34}, {1, 30}, {2, 24}, {100, 313134}, {350, 14045884}}) {
      auto [size, expected] = pair;
      std::vector<int> a(size);
      std::vector<int> b(size);
      for (int i = 0; i != size; ++i) {
        a[i] = i + 1;
        b[i] = i - 4;
      }

      decltype(auto) ret = std::transform_reduce(
          policy, Iter1(std::data(a)), Iter1(std::data(a) + std::size(a)), Iter2(std::data(b)), 34);
      static_assert(std::is_same_v<decltype(ret), int>);
      TC_ASSERT_EXPR(ret == expected);
    }
    {
      int a[] = {1, 2, 3, 4, 5, 6, 7, 8};
      int b[] = {8, 7, 6, 5, 4, 3, 2, 1};

      auto ret = std::transform_reduce(
          policy,
          Iter1(std::begin(a)),
          Iter1(std::end(a)),
          Iter2(std::begin(b)),
          constructible_from<int>{0},
          std::plus{},
          [](int i, int j) { return constructible_from<int>{i + j}; });
      TC_ASSERT_EXPR(ret.get() == 72);
    }
  }
};

int tc_algorithms_numeric_ops_transform_reduce_pstl_transform_reduce_binary(void) {
  types::for_each(
      types::forward_iterator_list<int*>{}, types::apply_type_identity{[](auto v) {
        using Iter2 = typename decltype(v)::type;
        types::for_each(
            types::forward_iterator_list<int*>{}, types::apply_type_identity{[](auto v2) {
              using Iter1 = typename decltype(v2)::type;
              types::for_each(
                  types::type_list<int, MoveOnly>{},
                  TestIteratorWithPolicies<types::partial_instantiation<Test, Iter1, Iter2>::template apply>{});
            }});
      }});

  return 0;
}
