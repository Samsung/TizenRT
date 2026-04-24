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
// UNSUPPORTED: c++03

// <functional>

// template<CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);    // constexpr since C++20
// template<Returnable R, CopyConstructible Fn, CopyConstructible... Types>
//   unspecified bind(Fn, Types...);    // constexpr since C++20

// https://llvm.org/PR23141
#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Fun {
  template<typename T, typename U>
  TEST_CONSTEXPR_CXX20 void operator()(T &&, U &&) const {
    static_assert(std::is_same<U, int &>::value, "");
  }
};

TEST_CONSTEXPR_CXX20 bool test() {
  std::bind(Fun{}, std::placeholders::_1, 42)("hello");
  return true;
}

int tc_utilities_function_objects_bind_func_bind_func_bind_bind_PR23141(void) {
  test();
#if TEST_STD_VER >= 20
  static_assert(test());
#endif

  return 0;
}
