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
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// test forward_like

#include <cassert>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

struct U {}; // class type so const-qualification is not stripped from a prvalue
using CU = const U;
using T  = int;
using CT = const T;

U u{};
const U& cu = u;

static_assert(std::is_same_v<decltype(std::forward_like<T>(U{})), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(CU{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(u)), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(cu)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(std::move(u))), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T>(std::move(cu))), CU&&>);

static_assert(std::is_same_v<decltype(std::forward_like<CT>(U{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(CU{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(u)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(cu)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(std::move(u))), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT>(std::move(cu))), CU&&>);

static_assert(std::is_same_v<decltype(std::forward_like<T&>(U{})), U&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(CU{})), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(u)), U&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(cu)), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(std::move(u))), U&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(std::move(cu))), CU&>);

static_assert(std::is_same_v<decltype(std::forward_like<CT&>(U{})), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(CU{})), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(u)), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(cu)), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(std::move(u))), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(std::move(cu))), CU&>);

static_assert(std::is_same_v<decltype(std::forward_like<T&&>(U{})), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(CU{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(u)), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(cu)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(std::move(u))), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(std::move(cu))), CU&&>);

static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(U{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(CU{})), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(u)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(cu)), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(std::move(u))), CU&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(std::move(cu))), CU&&>);

static_assert(noexcept(std::forward_like<T>(u)));

static_assert(std::is_same_v<decltype(std::forward_like<U&>(u)), U&>);
static_assert(std::is_same_v<decltype(std::forward_like<CU&>(cu)), CU&>);
static_assert(std::is_same_v<decltype(std::forward_like<U&&>(std::move(u))), U&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CU&&>(std::move(cu))), CU&&>);

struct NoCtorCopyMove {
  NoCtorCopyMove() = delete;
  NoCtorCopyMove(const NoCtorCopyMove&) = delete;
  NoCtorCopyMove(NoCtorCopyMove&&) = delete;
};

static_assert(std::is_same_v<decltype(std::forward_like<CT&&>(std::declval<NoCtorCopyMove>())), const NoCtorCopyMove&&>);
static_assert(std::is_same_v<decltype(std::forward_like<CT&>(std::declval<NoCtorCopyMove>())), const NoCtorCopyMove&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&&>(std::declval<NoCtorCopyMove>())), NoCtorCopyMove&&>);
static_assert(std::is_same_v<decltype(std::forward_like<T&>(std::declval<NoCtorCopyMove>())), NoCtorCopyMove&>);

static_assert(noexcept(std::forward_like<T>(std::declval<NoCtorCopyMove>())));

constexpr bool test() {
  {
    int val       = 1729;
    auto&& result = std::forward_like<const double&>(val);
    static_assert(std::is_same_v<decltype(result), const int&>);
    TC_ASSERT_EXPR(&result == &val);
  }
  {
    int val       = 1729;
    auto&& result = std::forward_like<double&>(val);
    static_assert(std::is_same_v<decltype(result), int&>);
    TC_ASSERT_EXPR(&result == &val);
  }
  {
    int val       = 1729;
    auto&& result = std::forward_like<const double&&>(val);
    static_assert(std::is_same_v<decltype(result), const int&&>);
    TC_ASSERT_EXPR(&result == &val);
  }
  {
    int val       = 1729;
    auto&& result = std::forward_like<double&&>(val);
    static_assert(std::is_same_v<decltype(result), int&&>);
    TC_ASSERT_EXPR(&result == &val);
  }
  return true;
}

int tc_utilities_utility_forward_forward_like(void) {
  test();
  static_assert(test());

  return 0;
}
