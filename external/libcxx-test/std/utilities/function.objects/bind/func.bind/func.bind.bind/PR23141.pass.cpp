//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
