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

// template <class... Types> class tuple;

// UNSUPPORTED: c++03

// Test the following constructors:
// (1) tuple(Types const&...)
// (2) tuple(UTypes&&...)
// Test that (1) short circuits before evaluating the copy constructor of the
// second argument. Constructor (2) should be selected.

#include <tuple>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NonConstCopyable {
  NonConstCopyable() = default;
  explicit NonConstCopyable(int v) : value(v) {}
  NonConstCopyable(NonConstCopyable&) = default;
  NonConstCopyable(NonConstCopyable const&) = delete;
  int value;
};

template <class T>
struct BlowsUpOnConstCopy {
  BlowsUpOnConstCopy() = default;
  constexpr BlowsUpOnConstCopy(BlowsUpOnConstCopy const&) {
      static_assert(!std::is_same<T, T>::value, "");
  }
  BlowsUpOnConstCopy(BlowsUpOnConstCopy&) = default;
};

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_test_lazy_sfinae(void) {
  NonConstCopyable v(42);
  BlowsUpOnConstCopy<int> b;
  std::tuple<NonConstCopyable, BlowsUpOnConstCopy<int>> t(v, b);
  TC_ASSERT_EXPR(std::get<0>(t).value == 42);

  return 0;
}
