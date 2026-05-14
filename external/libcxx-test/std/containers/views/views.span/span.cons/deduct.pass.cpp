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

// <span>

//   template<class It, class EndOrSize>
//     span(It, EndOrSize) -> span<remove_reference_t<iter_reference_t<_It>>>;
//
//   template<class T, size_t N>
//     span(T (&)[N]) -> span<T, N>;
//
//   template<class T, size_t N>
//     span(array<T, N>&) -> span<T, N>;
//
//   template<class T, size_t N>
//     span(const array<T, N>&) -> span<const T, N>;
//
//   template<class R>
//     span(R&&) -> span<remove_reference_t<ranges::range_reference_t<R>>>;


#include <span>
#include <array>
#include <cassert>
#include <iterator>
#include <memory>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

void test_iterator_sentinel() {
  int arr[] = {1, 2, 3};
  {
  std::span s{std::begin(arr), std::end(arr)};
  ASSERT_SAME_TYPE(decltype(s), std::span<int>);
  TC_ASSERT_EXPR(s.size() == std::size(arr));
  TC_ASSERT_EXPR(s.data() == std::data(arr));
  }
  {
  std::span s{std::begin(arr), 3};
  ASSERT_SAME_TYPE(decltype(s), std::span<int>);
  TC_ASSERT_EXPR(s.size() == std::size(arr));
  TC_ASSERT_EXPR(s.data() == std::data(arr));
  }
}

void test_c_array() {
    {
    int arr[] = {1, 2, 3};
    std::span s{arr};
    ASSERT_SAME_TYPE(decltype(s), std::span<int, 3>);
    TC_ASSERT_EXPR(s.size() == std::size(arr));
    TC_ASSERT_EXPR(s.data() == std::data(arr));
    }

    {
    const int arr[] = {1,2,3};
    std::span s{arr};
    ASSERT_SAME_TYPE(decltype(s), std::span<const int, 3>);
    TC_ASSERT_EXPR(s.size() == std::size(arr));
    TC_ASSERT_EXPR(s.data() == std::data(arr));
    }
}

void test_std_array() {
    {
    std::array<double, 4> arr = {1.0, 2.0, 3.0, 4.0};
    std::span s{arr};
    ASSERT_SAME_TYPE(decltype(s), std::span<double, 4>);
    TC_ASSERT_EXPR(s.size() == arr.size());
    TC_ASSERT_EXPR(s.data() == arr.data());
    }

    {
    const std::array<long, 5> arr = {4, 5, 6, 7, 8};
    std::span s{arr};
    ASSERT_SAME_TYPE(decltype(s), std::span<const long, 5>);
    TC_ASSERT_EXPR(s.size() == arr.size());
    TC_ASSERT_EXPR(s.data() == arr.data());
    }
}

void test_range_std_container() {
    {
    std::string str{"ABCDE"};
    std::span s{str};
    ASSERT_SAME_TYPE(decltype(s), std::span<char>);
    TC_ASSERT_EXPR(s.size() == str.size());
    TC_ASSERT_EXPR(s.data() == str.data());
    }

    {
    const std::string str{"QWERTYUIOP"};
    std::span s{str};
    ASSERT_SAME_TYPE(decltype(s), std::span<const char>);
    TC_ASSERT_EXPR(s.size() == str.size());
    TC_ASSERT_EXPR(s.data() == str.data());
    }
}

int tc_containers_views_views_span_span_cons_deduct(void) {
  test_iterator_sentinel();
  test_c_array();
  test_std_array();
  test_range_std_container();

  return 0;
}
