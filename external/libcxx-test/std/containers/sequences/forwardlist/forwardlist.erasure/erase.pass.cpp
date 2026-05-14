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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <forward_list>

// template <class T, class Allocator, class U>
//   typename forward_list<T, Allocator>::size_type
//   erase(forward_list<T, Allocator>& c, const U& value);

#include <forward_list>
#include <optional>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class S, class U>
void test0(S s, U val, S expected, std::size_t expected_erased_count) {
  ASSERT_SAME_TYPE(typename S::size_type, decltype(std::erase(s, val)));
  TC_ASSERT_EXPR(expected_erased_count == std::erase(s, val));
  TC_ASSERT_EXPR(s == expected);
}

template <class S>
void test()
{
  test0(S(), 1, S(), 0);

  test0(S({1}), 1, S(), 1);
  test0(S({1}), 2, S({1}), 0);

  test0(S({1, 2}), 1, S({2}), 1);
  test0(S({1, 2}), 2, S({1}), 1);
  test0(S({1, 2}), 3, S({1, 2}), 0);
  test0(S({1, 1}), 1, S(), 2);
  test0(S({1, 1}), 3, S({1, 1}), 0);

  test0(S({1, 2, 3}), 1, S({2, 3}), 1);
  test0(S({1, 2, 3}), 2, S({1, 3}), 1);
  test0(S({1, 2, 3}), 3, S({1, 2}), 1);
  test0(S({1, 2, 3}), 4, S({1, 2, 3}), 0);

  test0(S({1, 1, 1}), 1, S(), 3);
  test0(S({1, 1, 1}), 2, S({1, 1, 1}), 0);
  test0(S({1, 1, 2}), 1, S({2}), 2);
  test0(S({1, 1, 2}), 2, S({1, 1}), 1);
  test0(S({1, 1, 2}), 3, S({1, 1, 2}), 0);
  test0(S({1, 2, 2}), 1, S({2, 2}), 1);
  test0(S({1, 2, 2}), 2, S({1}), 2);
  test0(S({1, 2, 2}), 3, S({1, 2, 2}), 0);

  //  Test cross-type erasure
  using opt = std::optional<typename S::value_type>;
  test0(S({1, 2, 1}), opt(), S({1, 2, 1}), 0);
  test0(S({1, 2, 1}), opt(1), S({2}), 2);
  test0(S({1, 2, 1}), opt(2), S({1, 1}), 1);
  test0(S({1, 2, 1}), opt(3), S({1, 2, 1}), 0);
}

int tc_containers_sequences_forwardlist_forwardlist_erasure_erase(void) {
    test<std::forward_list<int>>();
    test<std::forward_list<int, min_allocator<int>>> ();
    test<std::forward_list<int, test_allocator<int>>> ();

    test<std::forward_list<long>>();
    test<std::forward_list<double>>();

  return 0;
}
