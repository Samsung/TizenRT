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
// <optional>

// template <class T, class U, class... Args>
//   constexpr optional<T> make_optional(initializer_list<U> il, Args&&... args);

#include <cassert>
#include <memory>
#include <optional>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct TestT {
  int x;
  int size;
  int *ptr;
  constexpr TestT(std::initializer_list<int> il)
    : x(*il.begin()), size(static_cast<int>(il.size())), ptr(nullptr) {}
  constexpr TestT(std::initializer_list<int> il, int *p)
    : x(*il.begin()), size(static_cast<int>(il.size())), ptr(p) {}
};

constexpr bool test()
{
  {
    auto opt = std::make_optional<TestT>({42, 2, 3});
    ASSERT_SAME_TYPE(decltype(opt), std::optional<TestT>);
    TC_ASSERT_EXPR(opt->x == 42);
    TC_ASSERT_EXPR(opt->size == 3);
    TC_ASSERT_EXPR(opt->ptr == nullptr);
  }
  {
    int i = 42;
    auto opt = std::make_optional<TestT>({42, 2, 3}, &i);
    ASSERT_SAME_TYPE(decltype(opt), std::optional<TestT>);
    TC_ASSERT_EXPR(opt->x == 42);
    TC_ASSERT_EXPR(opt->size == 3);
    TC_ASSERT_EXPR(opt->ptr == &i);
  }
  return true;
}

int tc_utilities_optional_optional_specalg_make_optional_explicit_initializer_list(void) {
  test();
  static_assert(test());
  {
    auto opt = std::make_optional<std::string>({'1', '2', '3'});
    TC_ASSERT_EXPR(*opt == "123");
  }
  {
    auto opt = std::make_optional<std::string>({'a', 'b', 'c'}, std::allocator<char>{});
    TC_ASSERT_EXPR(*opt == "abc");
  }
  return 0;
}
