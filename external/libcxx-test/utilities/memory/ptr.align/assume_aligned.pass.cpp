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

// #include <memory>

// template<size_t N, class T>
// [[nodiscard]] constexpr T* assume_aligned(T* ptr);

#include <memory>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <typename T>
constexpr void check(T* p) {
  ASSERT_SAME_TYPE(T*, decltype(std::assume_aligned<1>(p)));
  constexpr std::size_t alignment = alignof(T);

  if constexpr (alignment >= 1)
    TC_ASSERT_EXPR(p == std::assume_aligned<1>(p));
  if constexpr (alignment >= 2)
    TC_ASSERT_EXPR(p == std::assume_aligned<2>(p));
  if constexpr (alignment >= 4)
    TC_ASSERT_EXPR(p == std::assume_aligned<4>(p));
  if constexpr (alignment >= 8)
    TC_ASSERT_EXPR(p == std::assume_aligned<8>(p));
  if constexpr (alignment >= 16)
    TC_ASSERT_EXPR(p == std::assume_aligned<16>(p));
  if constexpr (alignment >= 32)
    TC_ASSERT_EXPR(p == std::assume_aligned<32>(p));
  if constexpr (alignment >= 64)
    TC_ASSERT_EXPR(p == std::assume_aligned<64>(p));
  if constexpr (alignment >= 128)
    TC_ASSERT_EXPR(p == std::assume_aligned<128>(p));
}

struct              S    { };
struct alignas(  4) S4   { };
struct alignas(  8) S8   { };
struct alignas( 16) S16  { };
struct alignas( 32) S32  { };
struct alignas( 64) S64  { };
struct alignas(128) S128 { };

constexpr bool tests() {
  char        c;
  int         i;
  long        l;
  double      d;
  long double ld;
  check( &c);
  check( &i);
  check( &l);
  check( &d);
  check(&ld);

  S    s;
  S4   s4;
  S8   s8;
  S16  s16;
  S32  s32;
  S64  s64;
  S128 s128;
  check(&s);
  check(&s4);
  check(&s8);
  check(&s16);
  check(&s32);
  check(&s64);
  check(&s128);

  return true;
}

int tc_utilities_memory_ptr_align_assume_aligned(void) {
  tests();
  static_assert(tests());

  return 0;
}
