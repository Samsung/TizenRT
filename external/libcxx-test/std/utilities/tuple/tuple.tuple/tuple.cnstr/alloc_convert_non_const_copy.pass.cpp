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
// <tuple>

// template <class... Types>
// template<class Alloc, class... UTypes>
//   constexpr explicit(see below)
//     tuple<Types...>::tuple(allocator_arg_t, const Alloc& a,
//     tuple<UTypes...>&);
//
// Constraints:
//  sizeof...(Types) equals sizeof...(UTypes) &&
//  (is_constructible_v<Types, decltype(get<I>(FWD(u)))> && ...) is true &&
//  (
//    sizeof...(Types) is not 1 ||
//    (
//      !is_convertible_v<decltype(u), T> &&
//      !is_constructible_v<T, decltype(u)> &&
//      !is_same_v<T, U>
//    )
//  )

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <tuple>

#include "copy_move_types.h"
#include "test_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// test: The expression inside explicit is equivalent to:
// !(is_convertible_v<decltype(get<I>(FWD(u))), Types> && ...)
static_assert(ImplicitlyConstructible< std::tuple<ConvertibleFrom<MutableCopy>>, std::allocator_arg_t,
                                       const test_allocator<int>&, std::tuple<MutableCopy>&>);

static_assert(
    ImplicitlyConstructible< std::tuple<ConvertibleFrom<MutableCopy>, ConvertibleFrom<MutableCopy>>,
                             std::allocator_arg_t, const test_allocator<int>&, std::tuple<MutableCopy, MutableCopy>&>);

static_assert(!ImplicitlyConstructible<std::tuple<ExplicitConstructibleFrom<MutableCopy>>, std::allocator_arg_t,
                                       const test_allocator<int>&, std::tuple<MutableCopy>&>);

static_assert(
    !ImplicitlyConstructible<std::tuple<ExplicitConstructibleFrom<MutableCopy>, ConvertibleFrom<MutableCopy>>,
                             std::allocator_arg_t, const test_allocator<int>&, std::tuple<MutableCopy, MutableCopy>&>);

constexpr bool test() {
  // test implicit conversions.
  // sizeof...(Types) == 1
  {
    std::tuple<MutableCopy> t1{1};
    std::tuple<ConvertibleFrom<MutableCopy>> t2 = {std::allocator_arg, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }

  // test implicit conversions.
  // sizeof...(Types) > 1
  {
    std::tuple<MutableCopy, int> t1{1, 2};
    std::tuple<ConvertibleFrom<MutableCopy>, int> t2 = {std::allocator_arg_t{}, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
    TC_ASSERT_EXPR(std::get<1>(t2) == 2);
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }

  // test explicit conversions.
  // sizeof...(Types) == 1
  {
    std::tuple<MutableCopy> t1{1};
    std::tuple<ExplicitConstructibleFrom<MutableCopy>> t2{std::allocator_arg_t{}, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }

  // test explicit conversions.
  // sizeof...(Types) > 1
  {
    std::tuple<MutableCopy, int> t1{1, 2};
    std::tuple<ExplicitConstructibleFrom<MutableCopy>, int> t2{std::allocator_arg_t{}, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
    TC_ASSERT_EXPR(std::get<1>(t2) == 2);
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }

  // test constraints

  // sizeof...(Types) != sizeof...(UTypes)
  static_assert(!std::is_constructible_v<std::tuple<int, int>, std::allocator_arg_t, const test_allocator<int>&,
                                         std::tuple<int>&>);
  static_assert(!std::is_constructible_v<std::tuple<int, int, int>, std::allocator_arg_t, const test_allocator<int>&,
                                         std::tuple<int, int>&>);

  // !(is_constructible_v<Types, decltype(get<I>(FWD(u)))> && ...)
  static_assert(!std::is_constructible_v< std::tuple<int, NoConstructorFromInt>, std::allocator_arg_t,
                                          const test_allocator<int>&, std::tuple<int, int>&>);

  // sizeof...(Types) == 1 && other branch of "||" satisfied
  {
    std::tuple<TracedCopyMove> t1{};
    std::tuple<ConvertibleFrom<TracedCopyMove>> t2{std::allocator_arg_t{}, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(nonConstCopyCtrCalled(std::get<0>(t2).v));
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }

  // sizeof...(Types) == 1 && is_same_v<T, U>
  {
    std::tuple<TracedCopyMove> t1{};
    std::tuple<TracedCopyMove> t2{std::allocator_arg_t{}, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(!nonConstCopyCtrCalled(std::get<0>(t2)));
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }

  // sizeof...(Types) != 1
  {
    std::tuple<TracedCopyMove, TracedCopyMove> t1{};
    std::tuple<TracedCopyMove, TracedCopyMove> t2{std::allocator_arg_t{}, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(nonConstCopyCtrCalled(std::get<0>(t2)));
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }

  // These two test points cause gcc to ICE
#if !defined(TEST_COMPILER_GCC)
  // sizeof...(Types) == 1 && is_convertible_v<decltype(u), T>
  {
    std::tuple<CvtFromTupleRef> t1{};
    std::tuple<ConvertibleFrom<CvtFromTupleRef>> t2{std::allocator_arg_t{}, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(!nonConstCopyCtrCalled(std::get<0>(t2).v));
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }

  // sizeof...(Types) == 1 && is_constructible_v<decltype(u), T>
  {
    std::tuple<ExplicitCtrFromTupleRef> t1{};
    std::tuple<ConvertibleFrom<ExplicitCtrFromTupleRef>> t2{std::allocator_arg_t{}, test_allocator<int>{}, t1};
    TC_ASSERT_EXPR(!nonConstCopyCtrCalled(std::get<0>(t2).v));
    TC_ASSERT_EXPR(std::get<0>(t2).alloc_constructed);
  }
#endif
  return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_alloc_convert_non_const_copy(void) {
  test();
  static_assert(test());
  return 0;
}
