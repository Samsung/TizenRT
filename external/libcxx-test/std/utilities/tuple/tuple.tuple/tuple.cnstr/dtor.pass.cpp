//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <tuple>

// template <class... Types> class tuple;

// ~tuple();

// C++17 added:
//   The destructor of tuple shall be a trivial destructor
//     if (is_trivially_destructible_v<Types> && ...) is true.

#include <tuple>
#include <string>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct TrackDtor {
   int* count_;
   constexpr explicit TrackDtor(int* count) : count_(count) {}
   TEST_CONSTEXPR_CXX14 TrackDtor(TrackDtor&& that) : count_(that.count_) { that.count_ = nullptr; }
   TEST_CONSTEXPR_CXX20 ~TrackDtor() { if(count_) ++*count_; }
};
static_assert(!std::is_trivially_destructible<TrackDtor>::value, "");

static_assert(std::is_trivially_destructible<std::tuple<>>::value, "");
static_assert(std::is_trivially_destructible<std::tuple<void*>>::value, "");
static_assert(std::is_trivially_destructible<std::tuple<int, float>>::value, "");
static_assert(!std::is_trivially_destructible<std::tuple<std::string>>::value, "");
static_assert(!std::is_trivially_destructible<std::tuple<int, std::string>>::value, "");

TEST_CONSTEXPR_CXX20 bool test() {
  int count = 0;
  {
    std::tuple<TrackDtor> tuple{TrackDtor(&count)};
    TC_ASSERT_EXPR(count == 0);
  }
  TC_ASSERT_EXPR(count == 1);

  return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_dtor(void) {
  test();
#if TEST_STD_VER > 17
  static_assert(test());
#endif

  return 0;
}
