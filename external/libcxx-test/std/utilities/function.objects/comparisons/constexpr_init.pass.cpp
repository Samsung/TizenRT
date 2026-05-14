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
// UNSUPPORTED: c++03, c++11

// <functional>

// equal_to, not_equal_to, less, et al.

// Test that these types can be constructed w/o an initializer in a constexpr
// context. This is specifically testing gcc.gnu.org/PR83921


#include <functional>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
constexpr bool test_constexpr_context() {
  std::equal_to<T> eq;
  ((void)eq);
  std::not_equal_to<T> neq;
  ((void)neq);
  std::less<T> l;
  ((void)l);
  std::less_equal<T> le;
  ((void)le);
  std::greater<T> g;
  ((void)g);
  std::greater_equal<T> ge;
  ((void)ge);
  return true;
}

static_assert(test_constexpr_context<int>(), "");
static_assert(test_constexpr_context<void>(), "");


int tc_utilities_function_objects_comparisons_constexpr_init(void) {


  return 0;
}
