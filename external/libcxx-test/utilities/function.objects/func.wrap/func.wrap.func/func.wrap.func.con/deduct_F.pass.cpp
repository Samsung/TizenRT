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
// <functional>

// template<class F>
// function(F) -> function<see-below>;

// UNSUPPORTED: c++03, c++11, c++14

#include <functional>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct R { };
struct A1 { };
struct A2 { };
struct A3 { };

#define DECLARE_FUNCTIONS_WITH_QUALS(N, ...)                              \
  struct f0_##N  { R operator()() __VA_ARGS__           { return {}; } }; \
  struct f1_##N  { R operator()(A1) __VA_ARGS__         { return {}; } }; \
  struct f2_##N  { R operator()(A1, A2) __VA_ARGS__     { return {}; } }; \
  struct f3_##N  { R operator()(A1, A2, A3) __VA_ARGS__ { return {}; } }  \
/**/

DECLARE_FUNCTIONS_WITH_QUALS(0, /* nothing */);
DECLARE_FUNCTIONS_WITH_QUALS(1, const);
DECLARE_FUNCTIONS_WITH_QUALS(2, volatile);
DECLARE_FUNCTIONS_WITH_QUALS(3, const volatile);
DECLARE_FUNCTIONS_WITH_QUALS(4, &);
DECLARE_FUNCTIONS_WITH_QUALS(5 , const &);
DECLARE_FUNCTIONS_WITH_QUALS(6 , volatile &);
DECLARE_FUNCTIONS_WITH_QUALS(7 , const volatile &);
DECLARE_FUNCTIONS_WITH_QUALS(8 , noexcept);
DECLARE_FUNCTIONS_WITH_QUALS(9 , const noexcept);
DECLARE_FUNCTIONS_WITH_QUALS(10, volatile noexcept);
DECLARE_FUNCTIONS_WITH_QUALS(11, const volatile noexcept);
DECLARE_FUNCTIONS_WITH_QUALS(12, & noexcept);
DECLARE_FUNCTIONS_WITH_QUALS(13, const & noexcept);
DECLARE_FUNCTIONS_WITH_QUALS(14, volatile & noexcept);
DECLARE_FUNCTIONS_WITH_QUALS(15, const volatile & noexcept);

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_deduct_F(void) {
#define CHECK_FUNCTIONS(N)                                                    \
  do {                                                                        \
    /* implicit */                                                            \
    std::function g0 = f0_##N{};                                              \
    ASSERT_SAME_TYPE(decltype(g0), std::function<R()>);                       \
                                                                              \
    std::function g1 = f1_##N{};                                              \
    ASSERT_SAME_TYPE(decltype(g1), std::function<R(A1)>);                     \
                                                                              \
    std::function g2 = f2_##N{};                                              \
    ASSERT_SAME_TYPE(decltype(g2), std::function<R(A1, A2)>);                 \
                                                                              \
    std::function g3 = f3_##N{};                                              \
    ASSERT_SAME_TYPE(decltype(g3), std::function<R(A1, A2, A3)>);             \
                                                                              \
    /* explicit */                                                            \
    std::function g4{f0_##N{}};                                               \
    ASSERT_SAME_TYPE(decltype(g4), std::function<R()>);                       \
                                                                              \
    std::function g5{f1_##N{}};                                               \
    ASSERT_SAME_TYPE(decltype(g5), std::function<R(A1)>);                     \
                                                                              \
    std::function g6{f2_##N{}};                                               \
    ASSERT_SAME_TYPE(decltype(g6), std::function<R(A1, A2)>);                 \
                                                                              \
    std::function g7{f3_##N{}};                                               \
    ASSERT_SAME_TYPE(decltype(g7), std::function<R(A1, A2, A3)>);             \
                                                                              \
    /* from std::function */                                                  \
    std::function<R(A1)> unary;                                               \
    std::function g8 = unary;                                                 \
    ASSERT_SAME_TYPE(decltype(g8), std::function<R(A1)>);                     \
                                                                              \
    std::function g9 = std::move(unary);                                      \
    ASSERT_SAME_TYPE(decltype(g9), std::function<R(A1)>);                     \
                                                                              \
    std::function<R(A1&&)> unary_ref;                                         \
    std::function g10 = unary_ref;                                            \
    ASSERT_SAME_TYPE(decltype(g10), std::function<R(A1&&)>);                  \
                                                                              \
    std::function g11 = std::move(unary_ref);                                 \
    ASSERT_SAME_TYPE(decltype(g11), std::function<R(A1&&)>);                  \
  } while (false)                                                             \
/**/

  // Make sure we can deduce from function objects with valid call operators
  CHECK_FUNCTIONS(0);
  CHECK_FUNCTIONS(1);
  CHECK_FUNCTIONS(2);
  CHECK_FUNCTIONS(3);
  CHECK_FUNCTIONS(4);
  CHECK_FUNCTIONS(5);
  CHECK_FUNCTIONS(6);
  CHECK_FUNCTIONS(7);
  CHECK_FUNCTIONS(8);
  CHECK_FUNCTIONS(9);
  CHECK_FUNCTIONS(10);
  CHECK_FUNCTIONS(11);
  CHECK_FUNCTIONS(12);
  CHECK_FUNCTIONS(13);
  CHECK_FUNCTIONS(14);
  CHECK_FUNCTIONS(15);

  return 0;
}

// Make sure we fail in a SFINAE-friendly manner when we try to deduce
// from a type without a valid call operator.
template <typename F, typename = decltype(std::function{std::declval<F>()})>
constexpr bool can_deduce() { return true; }
template <typename F>
constexpr bool can_deduce(...) { return false; }

struct invalid1 { };
struct invalid2 {
  template <typename ...Args>
  void operator()(Args ...);
};
struct invalid3 {
  void operator()(int);
  void operator()(long);
};
static_assert(!can_deduce<invalid1>());
static_assert(!can_deduce<invalid2>());
static_assert(!can_deduce<invalid3>());
